#include "stdafx_client.h"
#include "pragma/lua/classes/components/c_lentity_components.hpp"
#include "pragma/model/c_modelmesh.h"
#include <prosper_command_buffer.hpp>

void Lua::Animated::register_class(lua_State *l,luabind::module_ &entsMod)
{
	auto defCAnimated = luabind::class_<CAnimatedHandle,BaseEntityComponentHandle>("AnimatedComponent");
	Lua::register_base_animated_component_methods<luabind::class_<CAnimatedHandle,BaseEntityComponentHandle>,CAnimatedHandle>(l,defCAnimated);
	defCAnimated.def("GetEffectiveBoneTransform",static_cast<void(*)(lua_State*,CAnimatedHandle&,uint32_t)>([](lua_State *l,CAnimatedHandle &hAnim,uint32_t boneIdx) {
		pragma::Lua::check_component(l,hAnim);
		auto &transforms = hAnim->GetProcessedBones();
		if(boneIdx >= transforms.size())
			return;
		Lua::Push<pragma::physics::ScaledTransform*>(l,&transforms.at(boneIdx));
		}));
	defCAnimated.def("SetEffectiveBoneTransform",static_cast<void(*)(lua_State*,CAnimatedHandle&,uint32_t,const pragma::physics::ScaledTransform&)>([](lua_State *l,CAnimatedHandle &hAnim,uint32_t boneIdx,const pragma::physics::ScaledTransform &t) {
		pragma::Lua::check_component(l,hAnim);
		auto &transforms = hAnim->GetProcessedBones();
		if(boneIdx >= transforms.size())
			return;
		transforms.at(boneIdx) = t;
		}));
	defCAnimated.def("GetBoneBuffer",static_cast<void(*)(lua_State*,CAnimatedHandle&)>([](lua_State *l,CAnimatedHandle &hAnim) {
		pragma::Lua::check_component(l,hAnim);
		auto *pAnimComponent = hAnim.get();
		if(pAnimComponent == nullptr)
			return;
		auto buf = pAnimComponent->GetBoneBuffer();
		if(buf.expired())
			return;
		Lua::Push(l,buf.lock());
		}));
	defCAnimated.def("GetBoneRenderMatrices",static_cast<void(*)(lua_State*,CAnimatedHandle&)>([](lua_State *l,CAnimatedHandle &hAnim) {
		pragma::Lua::check_component(l,hAnim);
		auto *pAnimComponent = hAnim.get();
		if(pAnimComponent == nullptr)
			return;
		auto &mats = pAnimComponent->GetBoneMatrices();
		auto t = Lua::CreateTable(l);
		auto idx = 1u;
		for(auto &m : mats)
		{
			Lua::PushInt(l,idx++);
			Lua::Push<Mat4>(l,m);
			Lua::SetTableValue(l,t);
		}
		}));
	defCAnimated.def("GetBoneRenderMatrix",static_cast<void(*)(lua_State*,CAnimatedHandle&,uint32_t)>([](lua_State *l,CAnimatedHandle &hAnim,uint32_t boneIndex) {
		pragma::Lua::check_component(l,hAnim);
		auto *pAnimComponent = hAnim.get();
		if(pAnimComponent == nullptr)
			return;
		auto &mats = pAnimComponent->GetBoneMatrices();
		if(boneIndex >= mats.size())
			return;
		auto &m = mats.at(boneIndex);
		Lua::Push<Mat4>(l,m);
		}));
	defCAnimated.def("GetLocalVertexPosition",static_cast<void(*)(lua_State*,CAnimatedHandle&,std::shared_ptr<::ModelSubMesh>&,uint32_t)>([](lua_State *l,CAnimatedHandle &hAnim,std::shared_ptr<::ModelSubMesh> &subMesh,uint32_t vertexId) {
		pragma::Lua::check_component(l,hAnim);
		Vector3 pos;
		auto b = hAnim->GetLocalVertexPosition(static_cast<CModelSubMesh&>(*subMesh),vertexId,pos);
		if(b == false)
			return;
		Lua::Push<Vector3>(l,pos);
		}));
	defCAnimated.def("GetViewTarget",static_cast<void(*)(lua_State*,CAnimatedHandle&)>([](lua_State *l,CAnimatedHandle &hAnim) {
		pragma::Lua::check_component(l,hAnim);
		Lua::Push<Vector3>(l,hAnim->GetViewTarget());
	}));
	defCAnimated.def("SetViewTarget",static_cast<void(*)(lua_State*,CAnimatedHandle&,const Vector3&)>([](lua_State *l,CAnimatedHandle &hAnim,const Vector3 &viewTarget) {
		pragma::Lua::check_component(l,hAnim);
		hAnim->SetViewTarget(viewTarget);
	}));
	defCAnimated.def("GetEyeShift",static_cast<void(*)(lua_State*,CAnimatedHandle&,uint32_t)>([](lua_State *l,CAnimatedHandle &hAnim,uint32_t eyeIndex) {
		pragma::Lua::check_component(l,hAnim);
		auto *config = hAnim->GetEyeballConfig(eyeIndex);
		if(config == nullptr)
			return;
		Lua::Push<Vector3>(l,config->eyeShift);
	}));
	defCAnimated.def("SetEyeShift",static_cast<void(*)(lua_State*,CAnimatedHandle&,uint32_t,const Vector3&)>([](lua_State *l,CAnimatedHandle &hAnim,uint32_t eyeIndex,const Vector3 &eyeShift) {
		pragma::Lua::check_component(l,hAnim);
		auto *config = hAnim->GetEyeballConfig(eyeIndex);
		if(config == nullptr)
			return;
		config->eyeShift = eyeShift;
	}));
	defCAnimated.def("GetEyeJitter",static_cast<void(*)(lua_State*,CAnimatedHandle&,uint32_t)>([](lua_State *l,CAnimatedHandle &hAnim,uint32_t eyeIndex) {
		pragma::Lua::check_component(l,hAnim);
		auto *config = hAnim->GetEyeballConfig(eyeIndex);
		if(config == nullptr)
			return;
		Lua::Push<Vector2>(l,config->jitter);
	}));
	defCAnimated.def("SetEyeJitter",static_cast<void(*)(lua_State*,CAnimatedHandle&,uint32_t,const Vector2&)>([](lua_State *l,CAnimatedHandle &hAnim,uint32_t eyeIndex,const Vector2 &eyeJitter) {
		pragma::Lua::check_component(l,hAnim);
		auto *config = hAnim->GetEyeballConfig(eyeIndex);
		if(config == nullptr)
			return;
		config->jitter = eyeJitter;
	}));
	defCAnimated.def("GetEyeSize",static_cast<void(*)(lua_State*,CAnimatedHandle&,uint32_t)>([](lua_State *l,CAnimatedHandle &hAnim,uint32_t eyeIndex) {
		pragma::Lua::check_component(l,hAnim);
		auto *config = hAnim->GetEyeballConfig(eyeIndex);
		if(config == nullptr)
			return;
		Lua::PushNumber(l,config->eyeSize);
	}));
	defCAnimated.def("SetEyeSize",static_cast<void(*)(lua_State*,CAnimatedHandle&,uint32_t,float)>([](lua_State *l,CAnimatedHandle &hAnim,uint32_t eyeIndex,float eyeSize) {
		pragma::Lua::check_component(l,hAnim);
		auto *config = hAnim->GetEyeballConfig(eyeIndex);
		if(config == nullptr)
			return;
		config->eyeSize = eyeSize;
	}));
	defCAnimated.def("CalcEyeballPose",static_cast<void(*)(lua_State*,CAnimatedHandle&,uint32_t)>([](lua_State *l,CAnimatedHandle &hAnim,uint32_t eyeIndex) {
		pragma::Lua::check_component(l,hAnim);
		auto pose = hAnim->CalcEyeballPose(eyeIndex);
		Lua::Push(l,pose);
	}));
	defCAnimated.def("GetEyeballState",static_cast<void(*)(lua_State*,CAnimatedHandle&,uint32_t)>([](lua_State *l,CAnimatedHandle &hAnim,uint32_t eyeIndex) {
		pragma::Lua::check_component(l,hAnim);
		auto *eyeballData = hAnim->GetEyeballData(eyeIndex);
		if(eyeballData == nullptr)
			return;
		auto &eyeballState = eyeballData->state;
		Lua::Push<pragma::CAnimatedComponent::EyeballState*>(l,&eyeballState);
	}));
	defCAnimated.add_static_constant("EVENT_ON_SKELETON_UPDATED",pragma::CAnimatedComponent::EVENT_ON_SKELETON_UPDATED);
	defCAnimated.add_static_constant("EVENT_ON_BONE_MATRICES_UPDATED",pragma::CAnimatedComponent::EVENT_ON_BONE_MATRICES_UPDATED);
	defCAnimated.add_static_constant("EVENT_ON_BONE_BUFFER_INITIALIZED",pragma::CAnimatedComponent::EVENT_ON_BONE_BUFFER_INITIALIZED);
	defCAnimated.add_static_constant("EVENT_ON_EYEBALLS_UPDATED",pragma::CAnimatedComponent::EVENT_ON_EYEBALLS_UPDATED);

	auto defEyeballState = luabind::class_<pragma::CAnimatedComponent::EyeballState>("EyeballState");
	defEyeballState.def_readwrite("origin",&pragma::CAnimatedComponent::EyeballState::origin);
	defEyeballState.def_readwrite("forward",&pragma::CAnimatedComponent::EyeballState::forward);
	defEyeballState.def_readwrite("right",&pragma::CAnimatedComponent::EyeballState::right);
	defEyeballState.def_readwrite("up",&pragma::CAnimatedComponent::EyeballState::up);
	defEyeballState.def_readwrite("mat",&pragma::CAnimatedComponent::EyeballState::mat);
	defCAnimated.scope[defEyeballState];

	entsMod[defCAnimated];
}
