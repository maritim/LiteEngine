#include "SSAORenderPass.h"

#include "AmbientOcclusionMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/SSAOStatisticsObject.h"

bool SSAORenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if screen space ambient occlusion is enabled
	*/

	return settings.ssao_enabled;
}

std::string SSAORenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/AmbientOcclusion/screenSpaceAmbientOcclusionFragment.glsl";
}

std::string SSAORenderPass::GetPostProcessVolumeName () const
{
	return "SSAOMapVolume";
}

glm::ivec2 SSAORenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	if (_postProcessMapVolume != nullptr) {
		StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("SSAOStatisticsObject");
		SSAOStatisticsObject* ssaoStatisticsObject = nullptr;

		if (stat == nullptr) {
			stat = new SSAOStatisticsObject ();
			StatisticsManager::Instance ()->SetStatisticsObject ("SSAOStatisticsObject", stat);
		}

		ssaoStatisticsObject = dynamic_cast<SSAOStatisticsObject*> (stat);

		ssaoStatisticsObject->ssaoMapVolume = _postProcessMapVolume;
	}

	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.ssao_scale);
}

PostProcessMapVolume* SSAORenderPass::CreatePostProcessVolume () const
{
	AmbientOcclusionMapVolume* ssaoMapVolume = new AmbientOcclusionMapVolume ();

	return ssaoMapVolume;
}

std::vector<PipelineAttribute> SSAORenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute ssaoResolution;
	PipelineAttribute ssaoRadius;
	PipelineAttribute ssaoBias;

	ssaoResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	ssaoRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	ssaoBias.type = PipelineAttribute::AttrType::ATTR_1F;

	ssaoResolution.name = "ssaoResolution";
	ssaoRadius.name = "ssaoRadius";
	ssaoBias.name = "ssaoBias";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.ssao_scale);

	ssaoResolution.value = glm::vec3 (resolution, 0.0f);
	ssaoRadius.value.x = settings.ssao_radius;
	ssaoBias.value.x = settings.ssao_bias;

	attributes.push_back (ssaoResolution);
	attributes.push_back (ssaoRadius);
	attributes.push_back (ssaoBias);

	return attributes;
}
