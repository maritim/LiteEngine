#include "VoxelRadianceInjectionRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

VoxelRadianceInjectionRenderPass::VoxelRadianceInjectionRenderPass () :
	_firstTime (true)
{

}

void VoxelRadianceInjectionRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for voxel radiance injection render pass
	*/

	Resource<Shader> shader = Resources::LoadComputeShader ("Assets/Shaders/Voxelize/voxelRadianceInjectionCompute.glsl");

	_shaderView = RenderSystem::LoadComputeShader (shader);
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
	Pipeline::SetShader (_shaderView);
}

void VoxelRadianceInjectionRenderPass::RadianceInjectPass (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	auto shadowMapVolume = rvc->GetRenderVolume ("ShadowMapDirectionalLightVolume");
	auto voxelVolume = rvc->GetRenderVolume ("VoxelVolume");

	/*
	 * Bind render volumes for reading
	*/

	shadowMapVolume->BindForReading ();

	voxelVolume->BindForReading ();

	/*
	 * Send custom attributes of render volumes to pipeline
	*/

	Pipeline::SendCustomAttributes (_shaderView, shadowMapVolume->GetCustomAttributes ());

	Pipeline::SendCustomAttributes (_shaderView, voxelVolume->GetCustomAttributes ());

	/*
	 * Bind voxel volume for writing
	*/

	voxelVolume->BindForWriting ();

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
