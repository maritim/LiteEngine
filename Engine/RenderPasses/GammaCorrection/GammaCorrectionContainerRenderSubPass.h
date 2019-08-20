#ifndef GAMMACORRECTIONCONTAINERRENDERSUBPASS_H
#define GAMMACORRECTIONCONTAINERRENDERSUBPASS_H

#include "RenderPasses/PostProcess/PostProcessContainerRenderSubPass.h"

class ENGINE_API GammaCorrectionContainerRenderSubPass : public PostProcessContainerRenderSubPass
{
public:
	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	std::string GetPostProcessVolumeName () const;	
	glm::ivec2 GetPostProcessVolumeResolution (const RenderSettings& settings) const;
	PostProcessMapVolume* CreatePostProcessVolume () const;
};

#endif