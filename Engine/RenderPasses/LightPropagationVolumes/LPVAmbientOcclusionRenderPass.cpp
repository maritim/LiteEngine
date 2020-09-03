#include "LPVAmbientOcclusionRenderPass.h"

#include "RenderPasses/AmbientOcclusion/AmbientOcclusionMapVolume.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/LPVStatisticsObject.h"

bool LPVAmbientOcclusionRenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Always execure reflective shadow mapping indirect light render pass
	*/

	return true;
}

std::string LPVAmbientOcclusionRenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/LightPropagationVolumes/lightPropagationVolumesAmbientOcclusionFragment.glsl";
}

std::string LPVAmbientOcclusionRenderPass::GetPostProcessVolumeName () const
{
	return "LPVAOVolume";
}

glm::ivec2 LPVAmbientOcclusionRenderPass::GetPostProcessVolumeResolution (const RenderSettings& settings) const
{
	if (_postProcessMapVolume != nullptr) {
		StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("LPVStatisticsObject");
		LPVStatisticsObject* lpvStatisticsObject = nullptr;

		if (stat == nullptr) {
			stat = new LPVStatisticsObject ();
			StatisticsManager::Instance ()->SetStatisticsObject ("LPVStatisticsObject", stat);
		}

		lpvStatisticsObject = dynamic_cast<LPVStatisticsObject*> (stat);

		lpvStatisticsObject->lpvAmbientOcclusionMapVolume = _postProcessMapVolume;
	}

	return glm::ivec2 (settings.framebuffer.width, settings.framebuffer.height);
}

PostProcessMapVolume* LPVAmbientOcclusionRenderPass::CreatePostProcessVolume () const
{
	AmbientOcclusionMapVolume* lpvaoMapVolume = new AmbientOcclusionMapVolume ();

	return lpvaoMapVolume;
}

std::vector<PipelineAttribute> LPVAmbientOcclusionRenderPass::GetCustomAttributes (const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Attach post process volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = PostProcessRenderPass::GetCustomAttributes (camera, settings, rvc);

	/*
	 * Attach screen space ambient occlusion attributes to pipeline
	*/



	return attributes;
}
