#ifndef GAMMACORRECTIONCONTAINERRENDERSUBPASS_H
#define GAMMACORRECTIONCONTAINERRENDERSUBPASS_H

#include "RenderPasses/PostProcess/PostProcessContainerRenderSubPass.h"

class GammaCorrectionContainerRenderSubPass : public PostProcessContainerRenderSubPass
{
public:
	bool IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const;
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	std::string GetPostProcessVolumeName () const;	
	PostProcessMapVolume* CreatePostProcessVolume () const;
};

#endif