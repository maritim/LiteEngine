#include "LPVSampleWeightRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "LPVVolume.h"
#include "LPVSampleCountVolume.h"

void LPVSampleWeightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize post processing shader
	*/

	Resource<Shader> shader = Resources::LoadComputeShader (
		"Assets/Shaders/LightPropagationVolumes/lightPropagationVolumesSampleWeightCompute.glsl"
	);

	_shaderView = RenderSystem::LoadComputeShader (shader);
}

void LPVSampleWeightRenderPass::Clear ()
{
	/*
	 * Nothing
	*/
}

RenderVolumeCollection* LPVSampleWeightRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Start screen space ambient occlusion generation pass
	*/

	StartPostProcessPass ();

	/*
	 * Screen space ambient occlusion generation pass
	*/

	PostProcessPass (renderScene, camera, settings, rvc);

	/*
	 * End screen space ambient occlusion generation pass
	*/

	EndPostProcessPass ();

	return rvc;
}

bool LPVSampleWeightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute light propagation volumes radiance injection render pass
	*/

	return true;
}

void LPVSampleWeightRenderPass::StartPostProcessPass ()
{
	/*
	 * Bind screen space ambient occlusion volume for writing
	*/

	Pipeline::SetShader (_shaderView);
}

void LPVSampleWeightRenderPass::PostProcessPass (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	auto lpvVolume = (LPVVolume*) rvc->GetRenderVolume ("LightPropagationVolume");
	auto lpvSampleCountVolume = (LPVSampleCountVolume*) rvc->GetRenderVolume ("LPVSampleCountVolume");

	lpvVolume->BindForWriting ();

	Pipeline::SendCustomAttributes (_shaderView, lpvVolume->GetCustomAttributes ());
	Pipeline::SendCustomAttributes (_shaderView, lpvSampleCountVolume->GetCustomAttributes ());

	int numWorkGroups = (int) std::ceil (settings.lpv_volume_size / 4.0);
	GL::DispatchCompute (numWorkGroups, numWorkGroups, numWorkGroups);

	GL::MemoryBarrier (GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void LPVSampleWeightRenderPass::EndPostProcessPass ()
{
	/*
	 * Unlock current shader
	*/

	GL::MemoryBarrier (GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}
