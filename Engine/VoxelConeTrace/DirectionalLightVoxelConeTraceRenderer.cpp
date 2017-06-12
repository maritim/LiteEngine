#include "DirectionalLightVoxelConeTraceRenderer.h"

#include "Managers/ShaderManager.h"

DirectionalLightVoxelConeTraceRenderer::DirectionalLightVoxelConeTraceRenderer (Light* light) :
	DirectionalLightShadowMapRenderer (light),
	_rvc (nullptr)
{
	_shaderName = "VOXEL_CONE_TRACE_SHADOW_MAP_DIRECTIONAL_LIGHT";

	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/VoxelConeTrace/voxelConeTraceVertex.glsl",
		"Assets/Shaders/VoxelConeTrace/voxelConeTraceFragment.glsl");
}

DirectionalLightVoxelConeTraceRenderer::~DirectionalLightVoxelConeTraceRenderer ()
{
	
}

void DirectionalLightVoxelConeTraceRenderer::Draw (Scene* scene, Camera* camera, RenderVolumeCollection* rvc)
{
	/*
	 * Keep render volumes for later
	*/

	_rvc = rvc;

	/*
	 * Draw the scene with shadows
	*/

	LightShadowMapRenderer::Draw (scene, camera, rvc);
}

std::vector<PipelineAttribute> DirectionalLightVoxelConeTraceRenderer::GetCustomAttributes ()
{
	std::vector<PipelineAttribute> attributes = DirectionalLightShadowMapRenderer::GetCustomAttributes ();

	/*
	 * Attach all volume attributes to pipeline
	*/

	for (RenderVolumeI* renderVolume : *_rvc) {
		std::vector<PipelineAttribute> volumeAttributes = renderVolume->GetCustomAttributes ();

		attributes.insert (attributes.end (), volumeAttributes.begin (), volumeAttributes.end ());
	}

	//std::vector<PipelineAttribute> voxelAttributes = _voxelVolume->GetCustomAttributes ();

	//attributes.insert (attributes.begin (), voxelAttributes.begin (), voxelAttributes.end ());

	return attributes;
}