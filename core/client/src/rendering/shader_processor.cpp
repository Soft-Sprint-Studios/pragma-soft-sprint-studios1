/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2020 Florian Weischer
 */

#include "stdafx_client.h"
#include "pragma/rendering/render_processor.hpp"
#include "pragma/rendering/renderers/rasterization_renderer.hpp"
#include "pragma/rendering/shaders/world/c_shader_textured.hpp"
#include "pragma/rendering/shaders/world/c_shader_pbr.hpp"
#include "pragma/rendering/shaders/world/c_shader_prepass.hpp"
#include "pragma/rendering/shaders/world/c_shader_skybox.hpp"
#include "pragma/rendering/shaders/c_shader_shadow.hpp"
#include "pragma/entities/entity_instance_index_buffer.hpp"
#include "pragma/entities/components/c_scene_component.hpp"
#include "pragma/model/c_model.h"
#include <prosper_command_buffer.hpp>

extern DLLCENGINE CEngine *c_engine;
extern DLLCLIENT CGame *c_game;
#pragma optimize("",off)
bool pragma::rendering::ShaderProcessor::RecordBindScene(const pragma::CSceneComponent &scene,const pragma::CRasterizationRendererComponent &renderer,const pragma::ShaderGameWorld &shader,bool view)
{
	auto *dsScene = view ? scene.GetViewCameraDescriptorSet() : scene.GetCameraDescriptorSetGraphics();
	auto *dsRenderer = renderer.GetRendererDescriptorSet();
	auto &dsRenderSettings = c_game->GetGlobalRenderSettingsDescriptorSet();
	auto *dsLights = renderer.GetLightSourceDescriptorSet();
	auto *dsShadows = pragma::CShadowComponent::GetDescriptorSet();
	assert(dsScene);
	assert(dsRenderer);
	assert(dsLights);
	assert(dsShadows);
	auto &dsMat = shader.GetDefaultMaterialDescriptorSet();
	m_sceneFlags = ShaderGameWorld::SceneFlags::None;
	m_view = view;
	shader.RecordBindScene(
		*this,scene,renderer,
		*dsScene,*dsRenderer,dsRenderSettings,
		*dsLights,*dsShadows,dsMat,
		m_sceneFlags
	);
	return true;
}
bool pragma::rendering::ShaderProcessor::RecordBindShader(const pragma::CSceneComponent &scene,const pragma::CRasterizationRendererComponent &renderer,bool view,pragma::ShaderGameWorld &shader,uint32_t pipelineIdx)
{
	auto &context = c_engine->GetRenderContext();
	m_curShader = &shader;
	m_currentPipelineLayout = context.GetShaderPipelineLayout(shader,pipelineIdx);
	m_materialDescriptorSetIndex = m_curShader->GetMaterialDescriptorSetIndex();
	m_entityInstanceDescriptorSetIndex = m_curShader->GetInstanceDescriptorSetIndex();
	m_curInstanceSet = nullptr;
	m_clipPlane = {};
	m_depthBias = {};
	m_alphaCutoff = std::numeric_limits<float>::max();

	if(m_cmdBuffer.RecordBindShaderPipeline(shader,pipelineIdx) == false)
		return false;

#if 0
	// Reset depth bias
	// TODO: This kind of depth bias isn't used anymore and should be removed from the shaders entirely!
	m_cmdBuffer.RecordSetDepthBias();
#endif
	// m_cmdBuffer.RecordBindVertexBuffer(shader,*CSceneComponent::GetEntityInstanceIndexBuffer()->GetBuffer(),umath::to_integral(ShaderEntity::VertexBinding::RenderBufferIndex));
	return RecordBindScene(scene,renderer,shader,m_view);
}
void pragma::rendering::ShaderProcessor::UpdateSceneFlags(ShaderGameWorld::SceneFlags sceneFlags)
{
	if(sceneFlags == m_sceneFlags)
		return;
	m_sceneFlags = sceneFlags;
	m_curShader->RecordSceneFlags(*this,sceneFlags);
}
bool pragma::rendering::ShaderProcessor::RecordBindLight(CLightComponent &light,uint32_t layerId)
{
	m_curShader->RecordBindLight(*this,light,layerId);
	return true;
}
bool pragma::rendering::ShaderProcessor::RecordBindMaterial(CMaterial &mat)
{
	if(m_curShader->RecordBindMaterial(*this,mat) == false)
		return true;
	auto flags = m_sceneFlags;
	auto alphaMode = mat.GetAlphaMode();
	if(alphaMode != AlphaMode::Opaque)
	{
		auto alphaCutoff = mat.GetAlphaCutoff();
		if(alphaCutoff != m_alphaCutoff)
		{
			m_alphaCutoff = alphaCutoff;
			m_curShader->RecordAlphaCutoff(*this,alphaCutoff);
		}
		flags |= ShaderGameWorld::SceneFlags::AlphaTest;
	}

	// TODO
	//shaderScene->Set3DSky(umath::is_flag_set(m_renderFlags,RenderFlags::RenderAs3DSky));
	UpdateSceneFlags(flags);
	return true;
}
bool pragma::rendering::ShaderProcessor::RecordBindEntity(CBaseEntity &ent)
{
	auto *renderC = ent.GetRenderComponent();
	assert(renderC);
	auto *descSet = renderC->GetRenderDescriptorSet();
	assert(descSet);
	if(descSet == nullptr)
		return false;
	auto sceneFlags = m_sceneFlags;
	m_cmdBuffer.RecordBindDescriptorSets(prosper::PipelineBindPoint::Graphics,*m_currentPipelineLayout,m_entityInstanceDescriptorSetIndex,*descSet);

	auto *clipPlane = renderC->GetRenderClipPlane();
	if(static_cast<bool>(clipPlane) != m_clipPlane.has_value() && (!clipPlane || !m_clipPlane.has_value() || *clipPlane != *m_clipPlane))
	{
		auto vClipPlane = clipPlane ? *clipPlane : Vector4{};
		m_curShader->RecordClipPlane(*this,vClipPlane);

		m_clipPlane = clipPlane ? *clipPlane : std::optional<Vector4>{};
	}

	auto *depthBias = renderC->GetDepthBias();
	if(static_cast<bool>(depthBias) != m_depthBias.has_value() && (!depthBias || !m_depthBias.has_value() || *depthBias != *m_depthBias))
	{
		auto vDepthBias = depthBias ? *depthBias : Vector4{};
		m_curShader->RecordDepthBias(*this,vDepthBias);

		m_depthBias = depthBias ? *depthBias : std::optional<Vector2>{};
	}
	
	m_vertexAnimC = nullptr;
	m_modelC = renderC->GetModelComponent();
	if(m_modelC)
	{
		auto &vertAnimBuffer = static_cast<CModel&>(*m_modelC->GetModel()).GetVertexAnimationBuffer();
		if(vertAnimBuffer != nullptr)
			m_vertexAnimC = ent.GetComponent<pragma::CVertexAnimatedComponent>().get();
	}
	
	if(m_curShader->IsUsingLightmaps())
	{
		m_lightMapReceiverC = renderC->GetLightMapReceiverComponent();
		if(m_lightMapReceiverC)
			sceneFlags |= ShaderGameWorld::SceneFlags::LightmapsEnabled;
		umath::set_flag(sceneFlags,ShaderGameWorld::SceneFlags::DisableShadows,!renderC->IsReceivingShadows());
	}

	UpdateSceneFlags(sceneFlags);

	// TODO: Extended vertex weights
	// TODO: 3d sky
	return true;
}
bool pragma::rendering::ShaderProcessor::RecordDraw(CModelSubMesh &mesh,pragma::RenderMeshIndex meshIdx,const pragma::rendering::RenderQueue::InstanceSet *instanceSet)
{
	uint32_t vertexAnimationOffset = 0;
	if(m_vertexAnimC)
	{
		// TODO: Skip this if shader doesn't support morph target animations
		// (Check for specialization flag)
		auto offset = 0u;
		auto animCount = 0u;
		if(m_vertexAnimC->GetVertexAnimationBufferMeshOffset(mesh,offset,animCount) == true)
		{
			auto vaData = ((offset<<16)>>16) | animCount<<16;
			vertexAnimationOffset = vaData;
		}
	}
	if(vertexAnimationOffset != m_curVertexAnimationOffset)
	{
		m_curVertexAnimationOffset = vertexAnimationOffset;
		m_curShader->RecordVertexAnimationOffset(*this,vertexAnimationOffset);
	}
	// TODO
	// umath::set_flag(renderFlags,RenderFlags::UseExtendedVertexWeights,mesh.GetExtendedVertexWeights().empty() == false);

	auto numTriangleVertices = mesh.GetTriangleVertexCount();
	if(numTriangleVertices > umath::to_integral(GameLimits::MaxMeshVertices))
	{
		Con::cerr<<"ERROR: Attempted to draw mesh with more than maximum ("<<umath::to_integral(GameLimits::MaxMeshVertices)<<") amount of vertices!"<<Con::endl;
		return false;
	}
	auto &vkMesh = mesh.GetSceneMesh();
	auto &renderBuffer = m_modelC->GetRenderBuffer(meshIdx);
	if(renderBuffer == nullptr || m_cmdBuffer.RecordBindRenderBuffer(*renderBuffer) == false)
		return false;

	uint32_t instanceCount = 1;
	if(instanceSet)
	{
		instanceCount = instanceSet->instanceCount;
		if(instanceSet != m_curInstanceSet)
		{
			m_curInstanceSet = instanceSet;
			m_cmdBuffer.RecordBindVertexBuffer(*m_curShader,*instanceSet->instanceBuffer,umath::to_integral(ShaderEntity::VertexBinding::RenderBufferIndex));
		}
	}
	else if(m_curInstanceSet)
	{
		m_curInstanceSet = nullptr;
		m_cmdBuffer.RecordBindVertexBuffer(*m_curShader,*CSceneComponent::GetEntityInstanceIndexBuffer()->GetBuffer(),umath::to_integral(ShaderEntity::VertexBinding::RenderBufferIndex));
	}
	if(m_stats)
	{
		(*m_stats)->Increment(RenderPassStats::Counter::DrawnMeshes,instanceCount);
		(*m_stats)->Increment(RenderPassStats::Counter::DrawnVertices,mesh.GetVertexCount() *instanceCount);
		(*m_stats)->Increment(RenderPassStats::Counter::DrawnTriangles,mesh.GetTriangleCount() *instanceCount);
		m_stats->meshes.push_back(std::static_pointer_cast<CModelSubMesh>(mesh.shared_from_this()));

		(*m_stats)->Increment(RenderPassStats::Counter::DrawCalls);
	}
	m_curShader->OnRecordDrawMesh(*this,mesh);
	return m_cmdBuffer.RecordDrawIndexed(mesh.GetTriangleVertexCount(),instanceCount);
}
inline CBaseEntity &pragma::rendering::ShaderProcessor::GetCurrentEntity() const {return static_cast<CBaseEntity&>(m_modelC->GetEntity());}
#pragma optimize("",on)
