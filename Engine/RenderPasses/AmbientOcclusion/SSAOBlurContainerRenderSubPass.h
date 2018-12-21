#ifndef SSAOBLURCONTAINERRENDERSUBPASS_H
#define SSAOBLURCONTAINERRENDERSUBPASS_H

#include "RenderPasses/PostProcess/PostProcessContainerRenderSubPass.h"

class SSAOBlurContainerRenderSubPass : public PostProcessContainerRenderSubPass
{
public:
	bool IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const;
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	std::string GetPostProcessVolumeName () const;	
	PostProcessMapVolume* CreatePostProcessVolume () const;
};

#endif