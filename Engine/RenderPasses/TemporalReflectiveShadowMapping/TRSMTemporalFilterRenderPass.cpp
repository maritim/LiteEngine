#include "TRSMTemporalFilterRenderPass.h"

#include "TRSMIndirectDiffuseLightMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "TRSMStatisticsObject.h"

bool TRSMTemporalFilterRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return settings.trsm_temporal_filter_enabled;
}

std::string TRSMTemporalFilterRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/TemporalReflectiveShadowMapping/reflectiveShadowMapTemporalFilterFragment.glsl";
}

std::string TRSMTemporalFilterRenderPass::GetPostProcessVolumeName () const
{
	return "IndirectDiffuseMap";
}

glm::ivec2 TRSMTemporalFilterRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	return glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.rsm_scale);
}

FramebufferRenderVolume* TRSMTemporalFilterRenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create temporal reflective shadow mapping 
	*/

	Resource<Texture> texture = Resource<Texture> (new Texture (""));

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

	trsmStatisticsObject->trsmTemporalFilterMapVolume = renderVolume;

	return renderVolume;
}

std::vector<PipelineAttribute> TRSMTemporalFilterRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	auto rsmLastIndirectDiffuseMapVolume = (TRSMIndirectDiffuseLightMapVolume*) rvc->GetRenderVolume ("LastIndirectDiffuseMap");

	PipelineAttribute rsmResolution;
	PipelineAttribute reprojectionMatrix;

	rsmResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	reprojectionMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;

	rsmResolution.name = "rsmResolution";
	reprojectionMatrix.name = "reprojectionMatrix";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.resolution.width, settings.resolution.height) * settings.rsm_scale);

	rsmResolution.value = glm::vec3 (resolution, 0.0f);

	glm::mat4 viewMatrix = glm::translate (glm::mat4_cast (camera->GetRotation ()), camera->GetPosition () * -1.0f);
	glm::mat4 lastViewProjectionMatrix = rsmLastIndirectDiffuseMapVolume->GetViewProjectionMatrix ();
	glm::mat4 screenMatrix = glm::scale (glm::translate (glm::mat4 (1), glm::vec3 (0.5f)), glm::vec3 (0.5f));

	glm::mat4 reprojectionMat = screenMatrix * lastViewProjectionMatrix * glm::inverse (viewMatrix);

	reprojectionMatrix.matrix = reprojectionMat;

	attributes.push_back (rsmResolution);
	attributes.push_back (reprojectionMatrix);

	return attributes;
}
