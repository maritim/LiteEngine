#ifndef HORIZONTALGAUSSIANBLURCONTAINERRENDERSUBPASS_H
#define HORIZONTALGAUSSIANBLURCONTAINERRENDERSUBPASS_H

#include "RenderPasses/PostProcess/PostProcessContainerRenderSubPass.h"

class HorizontalGaussianBlurContainerRenderSubPass : public PostProcessContainerRenderSubPass
{
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	std::string GetPostProcessVolumeName () const;	
	PostProcessMapVolume* CreatePostProcessVolume () const;

	std::vector<PipelineAttribute> GetCustomAttributes (RenderVolumeCollection* rvc);
};

#endif