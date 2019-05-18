#ifndef VERTICALGAUSSIANBLURCONTAINERRENDERSUBPASS_H
#define VERTICALGAUSSIANBLURCONTAINERRENDERSUBPASS_H

#include "RenderPasses/PostProcess/PostProcessContainerRenderSubPass.h"

class ENGINE_API VerticalGaussianBlurContainerRenderSubPass : public PostProcessContainerRenderSubPass
{
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	std::string GetPostProcessVolumeName () const;	
	PostProcessMapVolume* CreatePostProcessVolume () const;

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc);
};

#endif