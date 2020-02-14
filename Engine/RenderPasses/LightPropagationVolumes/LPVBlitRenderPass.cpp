#include "LPVBlitRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Core/Console/Console.h"

LPVBlitRenderPass::LPVBlitRenderPass () :
	_lpvPropagationVolume (new LPVPropagationVolume ())
{

}

LPVBlitRenderPass::~LPVBlitRenderPass ()
{
	delete _lpvPropagationVolume;
}

void LPVBlitRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize post processing shader
	*/

	Resource<Shader> shader = Resources::LoadComputeShader (
		"Assets/Shaders/LightPropagationVolumes/lightPropagationVolumesBlitCompute.glsl"
	);

	_shaderView = RenderSystem::LoadComputeShader (shader);

	/*
	 * Initialize light propagation volume
	*/

	InitLPVVolume (settings);
}

void LPVBlitRenderPass::Clear ()
{
	/*
	 * Clear post processing volume
	*/

	_lpvPropagationVolume->Clear ();
}

RenderVolumeCollection* LPVBlitRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 *
	*/

	UpdateLPVVolume (settings);

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

	return rvc->Insert ("LightPropagationVolume", _lpvPropagationVolume);
}

bool LPVBlitRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute light propagation volumes radiance injection render pass
	*/

	return true;
}

void LPVBlitRenderPass::StartPostProcessPass ()
{
	_lpvPropagationVolume->ClearVolume ();

	/*
	 * Bind screen space ambient occlusion volume for writing
	*/

	Pipeline::SetShader (_shaderView);
}

void LPVBlitRenderPass::PostProcessPass (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	LPVVolume* lpvVolume = (LPVVolume*) rvc->GetRenderVolume ("LightPropagationVolume");

	_lpvPropagationVolume->UpdateBoundingBox (lpvVolume->GetMinVertex (), lpvVolume->GetMaxVertex ());

	_lpvPropagationVolume->BindForWriting ();

	Pipeline::SendCustomAttributes (_shaderView, lpvVolume->GetCustomAttributes ());

	int numWorkGroups = (int) std::ceil (settings.lpv_volume_size / 4.0);
	GL::DispatchCompute (numWorkGroups, numWorkGroups, numWorkGroups);

	GL::MemoryBarrier (GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void LPVBlitRenderPass::EndPostProcessPass ()
{
	/*
	 * Unlock current shader
	*/

	GL::MemoryBarrier (GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void LPVBlitRenderPass::InitLPVVolume (const RenderSettings& settings)
{
	if (!_lpvPropagationVolume->Init (settings.lpv_volume_size)) {
		Console::LogError (std::string () +
			"Light propagation volume texture cannot be initialized!" +
			" It is not possible to continue the process. End now!");
		exit (LIGHT_PROPAGATION_VOLUME_TEXTURE_NOT_INIT);
	}
}

void LPVBlitRenderPass::UpdateLPVVolume (const RenderSettings& settings)
{
	if (_lpvPropagationVolume->GetVolumeSize () != settings.lpv_volume_size) {

		/*
		 * Clear voxel volume
		*/

		_lpvPropagationVolume->Clear ();

		/*
		 * Initialize voxel volume
		*/

		InitLPVVolume (settings);
	}
}
