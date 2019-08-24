#ifndef HORIZONTALGAUSSIANBLURRENDERPASS_H
#define HORIZONTALGAUSSIANBLURRENDERPASS_H

#include "RenderPasses/PostProcess/PostProcessRenderPass.h"

class ENGINE_API HorizontalGaussianBlurRenderPass : public PostProcessRenderPass
{
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	std::string GetPostProcessVolumeName () const;	
	PostProcessMapVolume* CreatePostProcessVolume () const;

	std::vector<PipelineAttribute> GetCustomAttributes (const RenderSettings& settings, RenderVolumeCollection* rvc);
};

#endif