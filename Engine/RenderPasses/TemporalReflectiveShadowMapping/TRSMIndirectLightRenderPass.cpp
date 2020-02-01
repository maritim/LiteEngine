#include "TRSMIndirectLightRenderPass.h"

#include "TRSMIndirectLightMapVolume.h"

TRSMIndirectLightRenderPass::TRSMIndirectLightRenderPass () :
	_lastPostProcessMapVolume ()
{

}

TRSMIndirectLightRenderPass::~TRSMIndirectLightRenderPass ()
{
	delete _lastPostProcessMapVolume;
}

void TRSMIndirectLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize post process render pass
	*/

	PostProcessRenderPass::Init (settings);

	/*
	 * Initialize last post process map volume
	*/

	_lastPostProcessMapVolume = CreatePostProcessVolume ();

	InitTRSMIndirectMapVolume (settings);

	/*
	 * Initialize ping-pong buffers
	*/

	auto rsmIndirectMapVolume = (TRSMIndirectLightMapVolume*) _postProcessMapVolume;
	auto rsmLastIndirectMapVolume = (TRSMIndirectLightMapVolume*) _lastPostProcessMapVolume;

	rsmIndirectMapVolume->SetCurrent(true);
	rsmLastIndirectMapVolume->SetCurrent(false);
}

RenderVolumeCollection* TRSMIndirectLightRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update temporal anti-aliasing map volume
	*/

	UpdateTRSMIndirectMapVolume (settings);

	/*
	 * Execute post process render pass
	*/

	rvc = PostProcessRenderPass::Execute (renderScene, camera, settings, rvc);

	/*
	 * Keep current camera view projection matrix
	*/

	return rvc->Insert ("LastIndirectMap", _lastPostProcessMapVolume);
}

void TRSMIndirectLightRenderPass::Clear ()
{
	/*
	 * Clear last post processing volume
	*/

	_lastPostProcessMapVolume->Clear ();

	/*
	 * Clear post process render pass
	*/

	PostProcessRenderPass::Clear ();
}

bool TRSMIndirectLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return true;
}

std::string TRSMIndirectLightRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/TemporalReflectiveShadowMapping/temporalReflectiveShadowMapIndirectFragment.glsl";
}

PostProcessMapVolume* TRSMIndirectLightRenderPass::CreatePostProcessVolume () const
{
	TRSMIndirectLightMapVolume* trsmIndirectLightMapVolume = new TRSMIndirectLightMapVolume ();

	return trsmIndirectLightMapVolume;
}

void TRSMIndirectLightRenderPass::UpdateTRSMIndirectMapVolume (const RenderSettings& settings)
{
	glm::ivec2 framebufferSize = GetPostProcessVolumeResolution (settings);

	glm::ivec2 rsmIndirectMapSize = _lastPostProcessMapVolume->GetSize ();

	if (rsmIndirectMapSize != framebufferSize) {

		/*
		 * Clear temporal anti-aliasing map volume
		*/

		_lastPostProcessMapVolume->Clear ();

		/*
		 * Initialize temporal anti-aliasing map volume
		*/

		InitTRSMIndirectMapVolume (settings);
	}
}

void TRSMIndirectLightRenderPass::InitTRSMIndirectMapVolume (const RenderSettings& settings)
{
	/*
	 * Initialize temporal anti-aliasing map volume
	*/

	glm::ivec2 volumeResolution = GetPostProcessVolumeResolution (settings);

	if (!_lastPostProcessMapVolume->Init (volumeResolution)) {
		Console::LogError (std::string () + "Post-process volume cannot be initialized!" +
			"It is not possible to continue the process. End now!");
		exit (POST_PROCESS_MAP_VOLUME_NOT_INIT);
	}
}
