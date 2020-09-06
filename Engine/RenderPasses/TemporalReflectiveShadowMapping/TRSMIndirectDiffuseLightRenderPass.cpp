#include "TRSMIndirectDiffuseLightRenderPass.h"

#include "TRSMIndirectDiffuseLightMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "TRSMStatisticsObject.h"

#include "Core/Console/Console.h"

TRSMIndirectDiffuseLightRenderPass::TRSMIndirectDiffuseLightRenderPass () :
	_lastPostProcessMapVolume (nullptr)
{

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

	_lastPostProcessMapVolume = CreatePostProcessVolume (settings);

	/*
	 * Initialize ping-pong buffers
	*/

	auto trsmIndirectDiffuseMapVolume = (TRSMIndirectDiffuseLightMapVolume*) _postProcessMapVolume;
	auto trsmLastIndirectDiffuseMapVolume = (TRSMIndirectDiffuseLightMapVolume*) _lastPostProcessMapVolume;

	trsmIndirectDiffuseMapVolume->SetCurrent(true);
	trsmLastIndirectDiffuseMapVolume->SetCurrent(false);
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

	delete _lastPostProcessMapVolume;

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
	return "Assets/Shaders/TemporalReflectiveShadowMapping/temporalReflectiveShadowMapIndirectDiffuseFragment.glsl";
}

FramebufferRenderVolume* TRSMIndirectDiffuseLightRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create temporal reflective indirect diffuse light framebuffer
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("trsmIndirectDiffuseMap"));

	glm::ivec2 size = GetPostProcessVolumeResolution (settings);

	texture->SetSize (Size (size.x, size.y));
	texture->SetMipmapGeneration (false);
	texture->SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_RGB16);
	texture->SetInternalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_RGB);
	texture->SetChannelType (TEXTURE_CHANNEL_TYPE::CHANNEL_FLOAT);
	texture->SetWrapMode (TEXTURE_WRAP_MODE::WRAP_CLAMP_EDGE);
	texture->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	texture->SetMagFilter (TEXTURE_FILTER_MODE::FILTER_NEAREST);
	texture->SetAnisotropicFiltering (false);

	Resource<Framebuffer> framebuffer = Resource<Framebuffer> (new Framebuffer (texture));

	FramebufferRenderVolume* renderVolume = new TRSMIndirectDiffuseLightMapVolume (framebuffer);

	/*
	 * Update statistics object
	*/

	auto trsmStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <TRSMStatisticsObject> ();

	trsmStatisticsObject->trsmIndirectDiffuseMapVolume = renderVolume;

	return renderVolume;
}

void TRSMIndirectDiffuseLightRenderPass::UpdateTRSMIndirectDiffuseMapVolume (const RenderSettings& settings)
{
	glm::ivec2 volumeResolution = GetPostProcessVolumeResolution (settings);

	auto framebufferSize = _lastPostProcessMapVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();

	if ((std::size_t) volumeResolution.x != framebufferSize.width ||
		(std::size_t) volumeResolution.y != framebufferSize.height) {

		/*
		 * Clear temporal anti-aliasing map volume
		*/

		delete _lastPostProcessMapVolume;

		/*
		 * Initialize temporal anti-aliasing map volume
		*/

		_lastPostProcessMapVolume = CreatePostProcessVolume (settings);
	}
}
