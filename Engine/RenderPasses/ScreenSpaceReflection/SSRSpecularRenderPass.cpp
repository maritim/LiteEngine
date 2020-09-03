#include "SSRSpecularRenderPass.h"

#include "RenderPasses/IndirectSpecularLightMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/SSRStatisticsObject.h"

bool SSRSpecularRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space reflection is enabled
	*/

	return settings.ssr_enabled;
}

std::string SSRSpecularRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ScreenSpaceReflection/screenSpaceReflectionSpecularFragment.glsl";
}

std::string SSRSpecularRenderPass::GetPostProcessVolumeName () const
{
	return "SSRSpecularMapVolume";
}

glm::ivec2 SSRSpecularRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	if (_postProcessMapVolume != nullptr) {
		StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("SSRStatisticsObject");
		SSRStatisticsObject* ssrStatisticsObject = nullptr;

		if (stat == nullptr) {
			stat = new SSRStatisticsObject ();
			StatisticsManager::Instance ()->SetStatisticsObject ("SSRStatisticsObject", stat);
		}

		ssrStatisticsObject = dynamic_cast<SSRStatisticsObject*> (stat);

		ssrStatisticsObject->ssrMapVolume = _postProcessMapVolume;
	}

	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.ssr_scale);
}

PostProcessMapVolume* SSRSpecularRenderPass::CreatePostProcessVolume () const
{
	IndirectSpecularLightMapVolume* ssrMapVolume = new IndirectSpecularLightMapVolume ();

	return ssrMapVolume;
}

std::vector<PipelineAttribute> SSRSpecularRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssrIntensity;

	ssrIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	ssrIntensity.name = "ssrIntensity";

	ssrIntensity.value.x = settings.ssr_intensity;

	attributes.push_back (ssrIntensity);

	return attributes;
}
