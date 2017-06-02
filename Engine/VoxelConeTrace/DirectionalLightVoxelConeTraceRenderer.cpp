#include "DirectionalLightVoxelConeTraceRenderer.h"

#include "Managers/ShaderManager.h"

DirectionalLightVoxelConeTraceRenderer::DirectionalLightVoxelConeTraceRenderer (Light* light) :
	DirectionalLightShadowMapRenderer (light),
	_voxelVolume (nullptr)
{
	_shaderName = "VOXEL_CONE_TRACE_SHADOW_MAP_DIRECTIONAL_LIGHT";

	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/VoxelConeTrace/voxelConeTraceVertex.glsl",
		"Assets/Shaders/VoxelConeTrace/voxelConeTraceFragment.glsl");
}

DirectionalLightVoxelConeTraceRenderer::~DirectionalLightVoxelConeTraceRenderer ()
{
	
}

void DirectionalLightVoxelConeTraceRenderer::Draw (Scene* scene, Camera* camera, GBuffer* gBuffer, VoxelVolume* voxelVolume)
{
	/*
	 * Keep voxel volume for later
	*/

	_voxelVolume = voxelVolume;

	/*
	 * Draw the scene with shadows
	*/

	LightShadowMapRenderer::Draw (scene, camera, gBuffer, voxelVolume);
}

std::vector<PipelineAttribute> DirectionalLightVoxelConeTraceRenderer::GetCustomAttributes ()
{
	std::vector<PipelineAttribute> attributes = DirectionalLightShadowMapRenderer::GetCustomAttributes ();

	std::vector<PipelineAttribute> voxelAttributes = _voxelVolume->GetVoxelConeTracePipelineAttributes ();

	attributes.insert (attributes.begin (), voxelAttributes.begin (), voxelAttributes.end ());

	return attributes;
}