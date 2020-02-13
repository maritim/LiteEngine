#include "LPVPropagationRenderPass.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Core/Console/Console.h"

LPVPropagationRenderPass::LPVPropagationRenderPass () :
	_lpvVolume (new LPVVolume ())
{

}

LPVPropagationRenderPass::~LPVPropagationRenderPass ()
{
	delete _lpvVolume;
}

void LPVPropagationRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize post processing shader
	*/

	Resource<Shader> shader = Resources::LoadComputeShader (
		"Assets/Shaders/LightPropagationVolumes/lightPropagationVolumesPropagationCompute.glsl"
	);

	_shaderView = RenderSystem::LoadComputeShader (shader);

	/*
	 * Initialize light propagation volume
	*/

	InitLPVVolume (settings);
}

void LPVPropagationRenderPass::Clear ()
{
	/*
	 * Clear post processing volume
	*/

	_lpvVolume->Clear ();
}

RenderVolumeCollection* LPVPropagationRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
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

	if ((settings.lpv_iterations & 1) == 1) {
		return rvc->Insert ("LightPropagationVolume", _lpvVolume);
	}

	return rvc;
}

bool LPVPropagationRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute light propagation volumes radiance injection render pass
	*/

	return true;
}

void LPVPropagationRenderPass::StartPostProcessPass ()
{
	/*
	 * Bind screen space ambient occlusion volume for writing
	*/

	Pipeline::SetShader (_shaderView);
}

void LPVPropagationRenderPass::PostProcessPass (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	LPVVolume* lpvVolume = (LPVVolume*) rvc->GetRenderVolume ("LightPropagationVolume");

	for (std::size_t index = 0; index < settings.lpv_iterations; index ++) {

		if ((index & 1) == 0) {
			_lpvVolume->BindForWriting ();

			Pipeline::SendCustomAttributes (_shaderView, lpvVolume->GetCustomAttributes ());
		} else {
			lpvVolume->BindForWriting ();

			Pipeline::SendCustomAttributes (_shaderView, _lpvVolume->GetCustomAttributes ());
		}

		int numWorkGroups = (int) std::ceil (settings.lpv_volume_size / 4.0);
		GL::DispatchCompute (numWorkGroups, numWorkGroups, numWorkGroups);

		GL::MemoryBarrier (GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}
}

void LPVPropagationRenderPass::EndPostProcessPass ()
{
	/*
	 * Unlock current shader
	*/

	GL::MemoryBarrier (GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void LPVPropagationRenderPass::InitLPVVolume (const RenderSettings& settings)
{
	if (!_lpvVolume->Init (settings.lpv_volume_size)) {
		Console::LogError (std::string () +
			"Light propagation volume texture cannot be initialized!" +
			" It is not possible to continue the process. End now!");
		exit (LIGHT_PROPAGATION_VOLUME_TEXTURE_NOT_INIT);
	}
}

void LPVPropagationRenderPass::UpdateLPVVolume (const RenderSettings& settings)
{
	if (_lpvVolume->GetVolumeSize () != settings.lpv_volume_size) {

		/*
		 * Clear voxel volume
		*/

		_lpvVolume->Clear ();

		/*
		 * Initialize voxel volume
		*/

		InitLPVVolume (settings);
	}
}
