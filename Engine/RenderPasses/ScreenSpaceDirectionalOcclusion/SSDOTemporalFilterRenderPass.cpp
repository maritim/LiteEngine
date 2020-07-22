#include "SSDOTemporalFilterRenderPass.h"

#include "SSDOTemporalFilterMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/SSDOStatisticsObject.h"

bool SSDOTemporalFilterRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return settings.ssdo_enabled && settings.ssdo_temporal_filter_enabled;
}

std::string SSDOTemporalFilterRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ScreenSpaceDirectionalOcclusion/screenSpaceDirectionalOcclusionTemporalFilterFragment.glsl";
}

std::string SSDOTemporalFilterRenderPass::GetPostProcessVolumeName () const
{
	return "SSDOMapVolume";
}

PostProcessMapVolume* SSDOTemporalFilterRenderPass::CreatePostProcessVolume () const
{
	SSDOTemporalFilterMapVolume* ssdoTemporalFilterMapVolume = new SSDOTemporalFilterMapVolume ();

	return ssdoTemporalFilterMapVolume;
}

glm::ivec2 SSDOTemporalFilterRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
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

std::vector<PipelineAttribute> SSDOTemporalFilterRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = TemporalFilterRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space directional occlusion attributes to pipeline
	*/

	PipelineAttribute ssdoResolution;

	ssdoResolution.type = PipelineAttribute::AttrType::ATTR_2F;

	ssdoResolution.name = "ssdoResolution";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.ssdo_scale);

	ssdoResolution.value = glm::vec3 (resolution, 0.0f);

	attributes.push_back (ssdoResolution);

	return attributes;
}
