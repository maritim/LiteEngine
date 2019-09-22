#include "SSDORenderPass.h"

#include "SSDOMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/SSDOStatisticsObject.h"

bool SSDORenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execute screen space directional occlusion render pass
	*/

	return true;
}

std::string SSDORenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ScreenSpaceDirectionalOcclusion/screenSpaceDirectionalOcclusionFragment.glsl";
}

std::string SSDORenderPass::GetPostProcessVolumeName () const
{
	return "SSDOMapVolume";
}

glm::ivec2 SSDORenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	if (_postProcessMapVolume != nullptr) {
		StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("SSDOStatisticsObject");
		SSDOStatisticsObject* ssdoStatisticsObject = nullptr;

		if (stat == nullptr) {
			stat = new SSDOStatisticsObject ();
			StatisticsManager::Instance ()->SetStatisticsObject ("SSDOStatisticsObject", stat);
		}

		ssdoStatisticsObject = dynamic_cast<SSDOStatisticsObject*> (stat);

		ssdoStatisticsObject->ssdoMapVolume = _postProcessMapVolume;
	}

	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.ssdo_scale);
}

PostProcessMapVolume* SSDORenderPass::CreatePostProcessVolume () const
{
	SSDOMapVolume* ssdoMapVolume = new SSDOMapVolume ();

	return ssdoMapVolume;
}

std::vector<PipelineAttribute> SSDORenderPass::GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (settings, rvc);

	/*
	 * Attach screen space directional occlusion attributes to pipeline
	*/

	PipelineAttribute ssdoResolution;
	PipelineAttribute ssdoRadius;
	PipelineAttribute ssdoBias;
	PipelineAttribute ssdoIndirectIntensity;

	ssdoResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	ssdoRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	ssdoBias.type = PipelineAttribute::AttrType::ATTR_1F;
	ssdoIndirectIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	ssdoResolution.name = "ssdoResolution";
	ssdoRadius.name = "ssdoRadius";
	ssdoBias.name = "ssdoBias";
	ssdoIndirectIntensity.name = "ssdoIndirectIntensity";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.ssdo_scale);

	ssdoResolution.value = glm::vec3 (resolution, 0.0f);
	ssdoRadius.value.x = settings.ssdo_radius;
	ssdoBias.value.x = settings.ssdo_bias;
	ssdoIndirectIntensity.value.x = settings.ssdo_indirect_intensity;

	attributes.push_back (ssdoResolution);
	attributes.push_back (ssdoRadius);
	attributes.push_back (ssdoBias);
	attributes.push_back (ssdoIndirectIntensity);

	return attributes;
}
