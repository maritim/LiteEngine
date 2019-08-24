#include "VoxelRadianceInjectionRenderPass.h"

#include "Managers/ShaderManager.h"

#include "Renderer/Pipeline.h"

VoxelRadianceInjectionRenderPass::VoxelRadianceInjectionRenderPass () :
	_firstTime (true)
{

}

void VoxelRadianceInjectionRenderPass::Init (const RenderSettings& settings)
{
	ShaderManager::Instance ()->AddComputeShader ("VOXEL_RADIANCE_INJECTION_PASS_COMPUTE_SHADER",
		"Assets/Shaders/Voxelize/voxelRadianceInjectionCompute.glsl");
}

RenderVolumeCollection* VoxelRadianceInjectionRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	if (_firstTime || settings.vct_continuous_voxelization) {

		/*
		* Start radiance injecting pass
		*/

		StartRadianceInjectionPass ();

		/*
		* Radiance Injecting pass
		*/

		RadianceInjectPass (settings, rvc);

		/*
		* End radiance injecting pass
		*/

		EndRadianceInjectionPass ();

		_firstTime = false;
	}

	return rvc;
}

void VoxelRadianceInjectionRenderPass::Clear ()
{

}

void VoxelRadianceInjectionRenderPass::StartRadianceInjectionPass ()
{
	Pipeline::SetShader (ShaderManager::Instance ()->GetShader ("VOXEL_RADIANCE_INJECTION_PASS_COMPUTE_SHADER"));
}

void VoxelRadianceInjectionRenderPass::RadianceInjectPass (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Bind render volumes for reading
	*/

	rvc->GetRenderVolume ("ShadowMapDirectionalLightVolume")->BindForReading ();

	rvc->GetRenderVolume ("VoxelVolume")->BindForReading ();

	/*
	 * Send custom attributes of render volumes to pipeline
	*/

	Pipeline::SendCustomAttributes ("VOXEL_RADIANCE_INJECTION_PASS_COMPUTE_SHADER",
		rvc->GetRenderVolume ("ShadowMapDirectionalLightVolume")->GetCustomAttributes ());

	Pipeline::SendCustomAttributes ("VOXEL_RADIANCE_INJECTION_PASS_COMPUTE_SHADER",
		rvc->GetRenderVolume ("VoxelVolume")->GetCustomAttributes ());
	/*
	 * Bind voxel volume for writing
	*/

	rvc->GetRenderVolume ("VoxelVolume")->BindForWriting ();

	/*
	 * Inject radiance
	*/

	int numWorkGroups = (int) std::ceil (settings.vct_voxels_size / 4.0f);
	GL::DispatchCompute (numWorkGroups, numWorkGroups, numWorkGroups);
}

void VoxelRadianceInjectionRenderPass::EndRadianceInjectionPass ()
{
	/*
	* Make sure writing to image has finished before read
	*/

	GL::MemoryBarrier (GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

std::vector<PipelineAttribute> VoxelRadianceInjectionRenderPass::GetCustomAttributes (const RenderSettings& settings) const
{
	/*
	 * Attach voxel radiance injection attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes;

	PipelineAttribute shadowConeRatio;
	PipelineAttribute shadowConeDistance;

	shadowConeRatio.type = PipelineAttribute::AttrType::ATTR_1F;
	shadowConeDistance.type = PipelineAttribute::AttrType::ATTR_1F;

	shadowConeRatio.name = "shadowConeRatio";
	shadowConeDistance.name = "shadowConeDistance";

	shadowConeRatio.value.x = settings.vct_shadow_cone_ratio;
	shadowConeDistance.value.x = settings.vct_shadow_cone_distance;

	attributes.push_back (shadowConeRatio);
	attributes.push_back (shadowConeDistance);

	return attributes;
}
