#include "SSRRenderPass.h"

#include "SSRMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/SSRStatisticsObject.h"

bool SSRRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space reflection is enabled
	*/

	return settings.ssr_enabled;
}

std::string SSRRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ScreenSpaceReflection/screenSpaceReflectionFragment.glsl";
}

std::string SSRRenderPass::GetPostProcessVolumeName () const
{
	return "SSRMapVolume";
}

glm::ivec2 SSRRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	if (_postProcessMapVolume != nullptr) {
		StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("SSRStatisticsObject");
		SSRStatisticsObject* ssrStatisticsObject = nullptr;

		if (stat == nullptr) {
			stat = new SSRStatisticsObject ();
			StatisticsManager::Instance ()->SetStatisticsObject ("SSRStatisticsObject", stat);
		}

		ssrStatisticsObject = dynamic_cast<SSRStatisticsObject*> (stat);

		ssrStatisticsObject->ssrPositionMapVolume = _postProcessMapVolume;
	}

	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.ssr_scale);
}

PostProcessMapVolume* SSRRenderPass::CreatePostProcessVolume () const
{
	SSRMapVolume* ssrMapVolume = new SSRMapVolume ();

	return ssrMapVolume;
}

std::vector<PipelineAttribute> SSRRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssrResolution;
	PipelineAttribute ssrIterations;
	PipelineAttribute ssrRoughness;
	PipelineAttribute ssrThickness;
	PipelineAttribute ssrStride;

	ssrResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	ssrIterations.type = PipelineAttribute::AttrType::ATTR_1I;
	ssrRoughness.type = PipelineAttribute::AttrType::ATTR_1F;
	ssrThickness.type = PipelineAttribute::AttrType::ATTR_1F;
	ssrStride.type = PipelineAttribute::AttrType::ATTR_1I;

	ssrResolution.name = "ssrResolution";
	ssrIterations.name = "ssrIterations";
	ssrRoughness.name = "ssrRoughness";
	ssrThickness.name = "ssrThickness";
	ssrStride.name = "ssrStride";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.ssr_scale);

	ssrResolution.value = glm::vec3 (resolution, 0.0f);
	ssrIterations.value.x = settings.ssr_iterations;
	ssrRoughness.value.x = settings.ssr_roughness;
	ssrThickness.value.x = settings.ssr_thickness;
	ssrStride.value.x = settings.ssr_stride;

	attributes.push_back (ssrResolution);
	attributes.push_back (ssrIterations);
	attributes.push_back (ssrRoughness);
	attributes.push_back (ssrThickness);
	attributes.push_back (ssrStride);

	return attributes;
}
