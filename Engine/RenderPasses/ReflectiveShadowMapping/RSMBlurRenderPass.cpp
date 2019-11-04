#include "RSMBlurRenderPass.h"

#include "RSMIndirectLightMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/RSMStatisticsObject.h"

bool RSMBlurRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion blur is enabled
	*/

	return settings.rsm_blur_enabled;
}

std::string RSMBlurRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapBlurFragment.glsl";
}

std::string RSMBlurRenderPass::GetPostProcessVolumeName () const
{
	return "IndirectMap";
}

glm::ivec2 RSMBlurRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	if (_postProcessMapVolume != nullptr) {
		StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("RSMStatisticsObject");
		RSMStatisticsObject* rsmStatisticsObject = nullptr;

		if (stat == nullptr) {
			stat = new RSMStatisticsObject ();
			StatisticsManager::Instance ()->SetStatisticsObject ("RSMStatisticsObject", stat);
		}

		rsmStatisticsObject = dynamic_cast<RSMStatisticsObject*> (stat);

		rsmStatisticsObject->rsmIndirectMapVolume = _postProcessMapVolume;
	}

	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.rsm_scale);
}

PostProcessMapVolume* RSMBlurRenderPass::CreatePostProcessVolume () const
{
	RSMIndirectLightMapVolume* rsmIndirectLightMapVolume = new RSMIndirectLightMapVolume ();

	return rsmIndirectLightMapVolume;
}

std::vector<PipelineAttribute> RSMBlurRenderPass::GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute rsmResolution;

	rsmResolution.type = PipelineAttribute::AttrType::ATTR_2F;

	rsmResolution.name = "rsmResolution";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.rsm_scale);

	rsmResolution.value = glm::vec3 (resolution, 0.0f);

	attributes.push_back (rsmResolution);

	return attributes;
}
