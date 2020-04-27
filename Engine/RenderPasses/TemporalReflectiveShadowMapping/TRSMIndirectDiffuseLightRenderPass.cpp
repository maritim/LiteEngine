#include "TRSMIndirectDiffuseLightRenderPass.h"

#include "TRSMIndirectDiffuseLightMapVolume.h"

#include "Core/Console/Console.h"

TRSMIndirectDiffuseLightRenderPass::TRSMIndirectDiffuseLightRenderPass () :
	_lastPostProcessMapVolume ()
{

}

TRSMIndirectDiffuseLightRenderPass::~TRSMIndirectDiffuseLightRenderPass ()
{
	delete _lastPostProcessMapVolume;
}

void TRSMIndirectDiffuseLightRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Initialize post process render pass
	*/

	PostProcessRenderPass::Init (settings);

	/*
	 * Initialize last post process map volume
	*/

	_lastPostProcessMapVolume = CreatePostProcessVolume ();

	InitTRSMIndirectDiffuseMapVolume (settings);

	/*
	 * Initialize ping-pong buffers
	*/

	auto rsmIndirectDiffuseMapVolume = (TRSMIndirectDiffuseLightMapVolume*) _postProcessMapVolume;
	auto rsmLastIndirectDiffuseMapVolume = (TRSMIndirectDiffuseLightMapVolume*) _lastPostProcessMapVolume;

	rsmIndirectDiffuseMapVolume->SetCurrent(true);
	rsmLastIndirectDiffuseMapVolume->SetCurrent(false);
}

RenderVolumeCollection* TRSMIndirectDiffuseLightRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Update temporal anti-aliasing map volume
	*/

	UpdateTRSMIndirectDiffuseMapVolume (settings);

	/*
	 * Execute post process render pass
	*/

	rvc = PostProcessRenderPass::Execute (renderScene, camera, settings, rvc);

	/*
	 * Keep current camera view projection matrix
	*/

	return rvc->Insert ("LastIndirectDiffuseMap", _lastPostProcessMapVolume);
}

void TRSMIndirectDiffuseLightRenderPass::Clear ()
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

bool TRSMIndirectDiffuseLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return true;
}

std::string TRSMIndirectDiffuseLightRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/TemporalReflectiveShadowMapping/temporalReflectiveShadowMapIndirectFragment.glsl";
}

PostProcessMapVolume* TRSMIndirectDiffuseLightRenderPass::CreatePostProcessVolume () const
{
	TRSMIndirectDiffuseLightMapVolume* trsmIndirectDiffuseLightMapVolume = new TRSMIndirectDiffuseLightMapVolume ();

	return trsmIndirectDiffuseLightMapVolume;
}

void TRSMIndirectDiffuseLightRenderPass::UpdateTRSMIndirectDiffuseMapVolume (const RenderSettings& settings)
{
	glm::ivec2 framebufferSize = GetPostProcessVolumeResolution (settings);

	glm::ivec2 rsmIndirectDiffuseMapSize = _lastPostProcessMapVolume->GetSize ();

	if (rsmIndirectDiffuseMapSize != framebufferSize) {

		/*
		 * Clear temporal anti-aliasing map volume
		*/

		_lastPostProcessMapVolume->Clear ();

		/*
		 * Initialize temporal anti-aliasing map volume
		*/

		InitTRSMIndirectDiffuseMapVolume (settings);
	}
}

void TRSMIndirectDiffuseLightRenderPass::InitTRSMIndirectDiffuseMapVolume (const RenderSettings& settings)
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
