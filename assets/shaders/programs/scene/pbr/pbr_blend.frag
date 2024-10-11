#version 440

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#define DEBUG_MODE DEBUG_MODE_NONE

#include "/programs/scene/textured_blend.glsl"
#include "fs_core.glsl"

void main()
{
    vec2 uv = get_uv_coordinates();
    vec4 instanceColor = get_instance_color();

    vec4 albedoColor = fetch_albedo_map(uv, instanceColor);
    vec4 normalMap = fetch_normal_map(uv, instanceColor);
    vec3 rmaMap = fetch_rma_map(uv, instanceColor);

    if(u_pushConstants.alphaCount > 0) {
        vec4 albedoColor2 = fetch_albedo_map2(uv, instanceColor);
        vec4 normalMap2 = fetch_normal_map2(uv, instanceColor);
        vec3 rmaMap2 = fetch_rma_map2(uv, instanceColor);

        float blendFactor = fs_disp_in.alpha.x;
        albedoColor = albedoColor * (1.0 - blendFactor) + albedoColor2 * blendFactor;
        normalMap = normalMap * (1.0 - blendFactor) + normalMap2 * blendFactor;
        rmaMap = rmaMap * (1.0 - blendFactor) + rmaMap2 * blendFactor;
    }

    fs_color = calc_pbr_blend(albedoColor, uv, normalMap, rmaMap, u_pushConstants.debugMode);
    extract_bright_color(fs_color);
}