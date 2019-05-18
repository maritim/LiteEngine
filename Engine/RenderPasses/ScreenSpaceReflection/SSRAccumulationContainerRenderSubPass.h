#ifndef SSRACCUMULATIONCONTAINERRENDERSUBPASS_H
#define SSRACCUMULATIONCONTAINERRENDERSUBPASS_H

#include "RenderPasses/PostProcess/PostProcessContainerRenderSubPass.h"

class ENGINE_API SSRAccumulationContainerRenderSubPass : public PostProcessContainerRenderSubPass
{
public:
	bool IsAvailable (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	std::string GetPostProcessVolumeName () const;
	glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const;
	PostProcessMapVolume* CreatePostProcessVolume () const;

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc);
};

#endif