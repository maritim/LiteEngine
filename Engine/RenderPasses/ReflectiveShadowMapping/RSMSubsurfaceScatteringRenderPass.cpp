#include "RSMSubsurfaceScatteringRenderPass.h"

#include "RenderPasses/SubsurfaceScatteringMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/RSMStatisticsObject.h"

bool RSMSubsurfaceScatteringRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure light propagation volumes indirect light render pass
	*/

	return true;
}

std::string RSMSubsurfaceScatteringRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapSubsurfaceScatteringFragment.glsl";
}

std::string RSMSubsurfaceScatteringRenderPass::GetPostProcessVolumeName () const
{
	return "SubsurfaceScatteringMap";
}

glm::ivec2 RSMSubsurfaceScatteringRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	if (_postProcessMapVolume != nullptr) {
		StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("RSMStatisticsObject");
		RSMStatisticsObject* rsmStatisticsObject = nullptr;

		if (stat == nullptr) {
			stat = new RSMStatisticsObject ();
			StatisticsManager::Instance ()->SetStatisticsObject ("RSMStatisticsObject", stat);
		}

		rsmStatisticsObject = dynamic_cast<RSMStatisticsObject*> (stat);

		rsmStatisticsObject->rsmSubsurfaceScatteringMapVolume = _postProcessMapVolume;
	}

	return glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height);
}

PostProcessMapVolume* RSMSubsurfaceScatteringRenderPass::CreatePostProcessVolume () const
{
	SubsurfaceScatteringMapVolume* subsurfaceScatteringMapVolume = new SubsurfaceScatteringMapVolume ();

	return subsurfaceScatteringMapVolume;
}

std::vector<PipelineAttribute> RSMSubsurfaceScatteringRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach reflective shadow mapping subsurface scattering attributes to pipeline
	*/

	PipelineAttribute rsmResolution;
	PipelineAttribute rsmThickness;
	PipelineAttribute rsmSpecularIntensity;
	PipelineAttribute rsmIndirectRefractiveIntensity;

	rsmResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	rsmThickness.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmSpecularIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmIndirectRefractiveIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	rsmResolution.name = "rsmResolution";
	rsmThickness.name = "rsmThickness";
	rsmSpecularIntensity.name = "rsmSpecularIntensity";
	rsmIndirectRefractiveIntensity.name = "rsmIndirectRefractiveIntensity";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.rsm_scale);

	rsmResolution.value = glm::vec3 (resolution, 0.0f);
	rsmThickness.value.x = settings.rsm_thickness;
	rsmSpecularIntensity.value.x = settings.rsm_specular_intensity;
	rsmIndirectRefractiveIntensity.value.x = settings.rsm_indirect_refractive_intensity;

	attributes.push_back (rsmResolution);
	attributes.push_back (rsmThickness);
	attributes.push_back (rsmSpecularIntensity);
	attributes.push_back (rsmIndirectRefractiveIntensity);

	return attributes;
}
