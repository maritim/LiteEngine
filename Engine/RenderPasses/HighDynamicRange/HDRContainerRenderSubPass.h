#ifndef HDRCONTAINERRENDERSUBPASS_H
#define HDRCONTAINERRENDERSUBPASS_H

#include "RenderPasses/PostProcess/PostProcessContainerRenderSubPass.h"

class HDRContainerRenderSubPass : public PostProcessContainerRenderSubPass
{
public:
	bool IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const;
protected:
	std::string GetPostProcessFragmentShaderPath () const;
	std::string GetPostProcessVolumeName () const;	
	PostProcessMapVolume* CreatePostProcessVolume () const;
};

#endif