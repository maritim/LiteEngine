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
	 * Bind all render volumes
	*/

	// for (RenderVolumeI* renderVolume : *rvc) {
	// 	renderVolume->BindForReading ();
	// }

	/*
	 * Send custom attributes of render volumes to pipeline
	*/

	// for (RenderVolumeI* renderVolume : *rvc) {
	// 	Pipeline::SendCustomAttributes ("", renderVolume->GetCustomAttributes ());
	// }
	
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

	Pipeline::SendCustomAttributes ("VOXEL_RADIANCE_INJECTION_PASS_COMPUTE_SHADER", GetCustomAttributes ());

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

std::vector<PipelineAttribute> VoxelRadianceInjectionRenderPass::GetCustomAttributes () const
{
	/*
	 * Attach voxel radiance injection attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes;

	// PipelineAttribute ambientLightColor;
	// PipelineAttribute ambient;

	// ambientLightColor.type = PipelineAttribute::AttrType::ATTR_3F;
	// ambient.type = PipelineAttribute::AttrType::ATTR_3F;

	// ambientLightColor.name = "ambientLightColor";
	// ambient.name = "ambient";

	// // ambientLightColor.value = LightsManager::Instance ()->GetAmbientLightColor ().ToVector3 ();
	// ambient.value = _ambient ? glm::vec3 (1.0f) : glm::vec3 (0.0f);

	// attributes.push_back (ambientLightColor);
	// attributes.push_back (ambient);

	return attributes;
}
