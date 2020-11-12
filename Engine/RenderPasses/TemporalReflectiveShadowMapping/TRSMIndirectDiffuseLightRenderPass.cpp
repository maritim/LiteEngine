#include "TRSMIndirectDiffuseLightRenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "TRSMStatisticsObject.h"

std::string TRSMIndirectDiffuseLightRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/TemporalReflectiveShadowMapping/temporalReflectiveShadowMapIndirectDiffuseFragment.glsl";
}

FramebufferRenderVolume* TRSMIndirectDiffuseLightRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create temporal reflective indirect diffuse light framebuffer
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture ("indirectDiffuseMap"));

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

	FramebufferRenderVolume* renderVolume = new FramebufferRenderVolume (framebuffer);

	/*
	 * Update statistics object
	*/

	auto trsmStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <TRSMStatisticsObject> ();

	trsmStatisticsObject->trsmIndirectDiffuseMapVolume = renderVolume;

	return renderVolume;
}
