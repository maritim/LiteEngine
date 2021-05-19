#include "TAARenderPass.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "TAAStatisticsObject.h"

bool TAARenderPass::IsAvailable (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, const RenderVolumeCollection* rvc) const
{
	/*
	 * Check if temporal anti-aliasing is enabled
	*/

	return settings.taa_enabled;
}

std::string TAARenderPass::GetPostProcessFragmentShaderPath () const
{
	return "Assets/Shaders/TemporalAntialiasing/temporalAntialiasingFragment.glsl";
}

FramebufferRenderVolume* TAARenderPass::CreatePostProcessVolume (const RenderSettings& settings) const
{
	/*
	 * Create screen space reflections framebuffer
	*/

	auto renderVolume = TemporalFilterRenderPass::CreatePostProcessVolume (settings);

	/*
	 * Update statistics object
	*/

	auto taaStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <TAAStatisticsObject> ();

	taaStatisticsObject->taaMapVolume = renderVolume;

	return renderVolume;
}
