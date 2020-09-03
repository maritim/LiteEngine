#include "RSMIndirectDiffuseLightRenderPass.h"

#include "RenderPasses/IndirectDiffuseLightMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/RSMStatisticsObject.h"

#include "Utils/Extensions/MathExtend.h"

bool RSMIndirectDiffuseLightRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return true;
}

std::string RSMIndirectDiffuseLightRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapIndirectDiffuseFragment.glsl";
}

std::string RSMIndirectDiffuseLightRenderPass::GetPostProcessVolumeName () const
{
	return "IndirectDiffuseMap";
}

glm::ivec2 RSMIndirectDiffuseLightRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	if (_postProcessMapVolume != nullptr) {
		StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("RSMStatisticsObject");
		RSMStatisticsObject* rsmStatisticsObject = nullptr;

		if (stat == nullptr) {
			stat = new RSMStatisticsObject ();
			StatisticsManager::Instance ()->SetStatisticsObject ("RSMStatisticsObject", stat);
		}

		rsmStatisticsObject = dynamic_cast<RSMStatisticsObject*> (stat);

		rsmStatisticsObject->rsmIndirectDiffuseMapVolume = _postProcessMapVolume;
	}

	return glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.rsm_scale);
}

PostProcessMapVolume* RSMIndirectDiffuseLightRenderPass::CreatePostProcessVolume () const
{
	IndirectDiffuseLightMapVolume* indirectDiffuseLightMapVolume = new IndirectDiffuseLightMapVolume();

	return indirectDiffuseLightMapVolume;
}

std::vector<PipelineAttribute> RSMIndirectDiffuseLightRenderPass::GetCustomAttributes (const Camera* camera,
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
	PipelineAttribute rsmRadius;
	PipelineAttribute rsmIntensity;
	PipelineAttribute rsmInterpolationScale;
	PipelineAttribute rsmMinInterpolationDistance;
	PipelineAttribute rsmMinInterpolationAngle;
	PipelineAttribute rsmDebugInterpolation;

	rsmResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	rsmRadius.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmInterpolationScale.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmMinInterpolationDistance.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmMinInterpolationAngle.type = PipelineAttribute::AttrType::ATTR_1F;
	rsmDebugInterpolation.type = PipelineAttribute::AttrType::ATTR_1I;

	rsmResolution.name = "rsmResolution";
	rsmRadius.name = "rsmRadius";
	rsmIntensity.name = "rsmIntensity";
	rsmInterpolationScale.name = "rsmInterpolationScale";
	rsmMinInterpolationDistance.name = "rsmMinInterpolationDistance";
	rsmMinInterpolationAngle.name = "rsmMinInterpolationAngle";
	rsmDebugInterpolation.name = "rsmDebugInterpolation";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.rsm_scale);

	rsmResolution.value = glm::vec3 (resolution, 0.0f);
	rsmRadius.value.x = settings.rsm_radius;
	rsmIntensity.value.x = settings.rsm_intensity;
	rsmInterpolationScale.value.x = settings.rsm_interpolation_scale;
	rsmMinInterpolationDistance.value.x = settings.rsm_min_interpolation_distance;
	rsmMinInterpolationAngle.value.x = std::cos (DEG2RAD * settings.rsm_min_interpolation_angle);
	rsmDebugInterpolation.value.x = settings.rsm_debug_interpolation;

	attributes.push_back (rsmResolution);
	attributes.push_back (rsmRadius);
	attributes.push_back (rsmIntensity);
	attributes.push_back (rsmInterpolationScale);
	attributes.push_back (rsmMinInterpolationDistance);
	attributes.push_back (rsmMinInterpolationAngle);
	attributes.push_back (rsmDebugInterpolation);

	return attributes;
}
