#include "VoxelConeTraceDirectionalLightRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

VoxelConeTraceDirectionalLightRenderPass::VoxelConeTraceDirectionalLightRenderPass () :
	_shadowShaderName ("VOXEL_CONE_TRACE_SHADOW_MAP_DIRECTIONAL_LIGHT")
{

}

void VoxelConeTraceDirectionalLightRenderPass::Init ()
{
	/*
	 * Shader for general directional light with no shadow casting
	*/

	ShaderManager::Instance ()->AddShader (_shadowShaderName,
		"Assets/Shaders/VoxelConeTrace/voxelConeTraceVertex.glsl",
		"Assets/Shaders/VoxelConeTrace/voxelConeTraceFragment.glsl");
}

void VoxelConeTraceDirectionalLightRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

void VoxelConeTraceDirectionalLightRenderPass::LockShader (const VolumetricLight* volumetricLight)
{
	/*
	 * Unlock last shader
	*/

	Pipeline::UnlockShader ();

	/*
	 * Lock shader for shadow directional light
	*/

	Pipeline::LockShader (ShaderManager::Instance ()->GetShader (_shadowShaderName));
}

std::vector<PipelineAttribute> VoxelConeTraceDirectionalLightRenderPass::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	return attributes;
}
