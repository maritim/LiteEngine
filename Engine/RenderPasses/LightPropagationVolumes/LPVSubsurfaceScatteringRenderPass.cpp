#include "LPVSubsurfaceScatteringRenderPass.h"

#include "RenderPasses/SubsurfaceScatteringMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/LPVStatisticsObject.h"

bool LPVSubsurfaceScatteringRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure light propagation volumes indirect light render pass
	*/

	return true;
}

std::string LPVSubsurfaceScatteringRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/LightPropagationVolumes/lightPropagationVolumesSubsurfaceScatteringFragment.glsl";
}

std::string LPVSubsurfaceScatteringRenderPass::GetPostProcessVolumeName () const
{
	return "SubsurfaceScatteringMap";
}

glm::ivec2 LPVSubsurfaceScatteringRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	if (_postProcessMapVolume != nullptr) {
		StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("LPVStatisticsObject");
		LPVStatisticsObject* lpvStatisticsObject = nullptr;

		if (stat == nullptr) {
			stat = new LPVStatisticsObject ();
			StatisticsManager::Instance ()->SetStatisticsObject ("LPVStatisticsObject", stat);
		}

		lpvStatisticsObject = dynamic_cast<LPVStatisticsObject*> (stat);

		lpvStatisticsObject->lpvSubsurfaceScatteringMapVolume = _postProcessMapVolume;
	}

	return glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height);
}

PostProcessMapVolume* LPVSubsurfaceScatteringRenderPass::CreatePostProcessVolume () const
{
	SubsurfaceScatteringMapVolume* subsurfaceScatteringMapVolume = new SubsurfaceScatteringMapVolume ();

	return subsurfaceScatteringMapVolume;
}

std::vector<PipelineAttribute> LPVSubsurfaceScatteringRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/

	PipelineAttribute lpvIndirectRefractiveIntensity;

	lpvIndirectRefractiveIntensity.type = PipelineAttribute::AttrType::ATTR_1F;

	lpvIndirectRefractiveIntensity.name = "lpvIndirectRefractiveIntensity";

	lpvIndirectRefractiveIntensity.value.x = settings.lpv_indirect_refractive_intensity;

	attributes.push_back (lpvIndirectRefractiveIntensity);

	return attributes;
}
