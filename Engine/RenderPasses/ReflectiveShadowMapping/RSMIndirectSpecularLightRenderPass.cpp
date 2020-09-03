#include "RSMIndirectSpecularLightRenderPass.h"

#include "RenderPasses/IndirectSpecularLightMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/RSMStatisticsObject.h"

bool RSMIndirectSpecularLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return true;
}

std::string RSMIndirectSpecularLightRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapIndirectSpecularFragment.glsl";
}

std::string RSMIndirectSpecularLightRenderPass::GetPostProcessVolumeName () const
{
	return "IndirectSpecularMap";
}

glm::ivec2 RSMIndirectSpecularLightRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	if (_postProcessMapVolume != nullptr) {
		StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("RSMStatisticsObject");
		RSMStatisticsObject* rsmStatisticsObject = nullptr;

		if (stat == nullptr) {
			stat = new RSMStatisticsObject ();
			StatisticsManager::Instance ()->SetStatisticsObject ("RSMStatisticsObject", stat);
		}

		rsmStatisticsObject = dynamic_cast<RSMStatisticsObject*> (stat);

		rsmStatisticsObject->rsmIndirectSpecularMapVolume = _postProcessMapVolume;
	}

	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.rsm_scale);
}

PostProcessMapVolume* RSMIndirectSpecularLightRenderPass::CreatePostProcessVolume () const
{
	IndirectSpecularLightMapVolume* indirectSpecularLightMapVolume = new IndirectSpecularLightMapVolume();

	return indirectSpecularLightMapVolume;
}

std::vector<PipelineAttribute> RSMIndirectSpecularLightRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute rsmResolution;
	PipelineAttribute rsmThickness;
	PipelineAttribute rsmSpecularIntensity;

	rsmResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	rsmThickness.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmSpecularIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	rsmResolution.name = "rsmResolution";
	rsmThickness.name = "rsmThickness";
	rsmSpecularIntensity.name = "rsmSpecularIntensity";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.rsm_scale);

	rsmResolution.value = glm::vec3 (resolution, 0.0f);
	rsmThickness.value.x = settings.rsm_thickness;
	rsmSpecularIntensity.value.x = settings.rsm_specular_intensity;

	attributes.push_back (rsmResolution);
	attributes.push_back (rsmThickness);
	attributes.push_back (rsmSpecularIntensity);

	return attributes;
}
