#include "TRSMTemporalFilterRenderPass.h"

#include "TRSMIndirectLightMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/RSMStatisticsObject.h"

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
	return "Assets/Shaders/ReflectiveShadowMapping/reflectiveShadowMapTemporalFilterFragment.glsl";
}

std::string TRSMTemporalFilterRenderPass::GetPostProcessVolumeName () const
{
	return "IndirectMap";
}

glm::ivec2 TRSMTemporalFilterRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
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

PostProcessMapVolume* TRSMTemporalFilterRenderPass::CreatePostProcessVolume () const
{
	TRSMIndirectLightMapVolume* trsmIndirectLightMapVolume = new TRSMIndirectLightMapVolume ();

	return trsmIndirectLightMapVolume;
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

	auto rsmLastIndirectMapVolume = (TRSMIndirectLightMapVolume*) rvc->GetRenderVolume ("LastIndirectMap");

	PipelineAttribute rsmResolution;
	PipelineAttribute reprojectionMatrix;

	rsmResolution.type = PipelineAttribute::AttrType::ATTR_2F;
	reprojectionMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;

	rsmResolution.name = "rsmResolution";
	reprojectionMatrix.name = "reprojectionMatrix";

	glm::ivec2 resolution = glm::ivec2 (glm::vec2 (settings.framebuffer.width, settings.framebuffer.height) * settings.rsm_scale);

	rsmResolution.value = glm::vec3 (resolution, 0.0f);

	glm::mat4 viewMatrix = glm::translate (glm::mat4_cast (camera->GetRotation ()), camera->GetPosition () * -1.0f);
	glm::mat4 lastViewProjectionMatrix = rsmLastIndirectMapVolume->GetViewProjectionMatrix ();
	glm::mat4 screenMatrix = glm::scale (glm::translate (glm::mat4 (1), glm::vec3 (0.5f)), glm::vec3 (0.5f));

	glm::mat4 reprojectionMat = screenMatrix * lastViewProjectionMatrix * glm::inverse (viewMatrix);

	reprojectionMatrix.matrix = reprojectionMat;

	attributes.push_back (rsmResolution);
	attributes.push_back (reprojectionMatrix);

	return attributes;
}
