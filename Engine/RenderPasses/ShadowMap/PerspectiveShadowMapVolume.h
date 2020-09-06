#ifndef PERSPECTIVESHADOWMAPVOLUME_H
#define PERSPECTIVESHADOWMAPVOLUME_H

#include "RenderPasses/FramebufferRenderVolume.h"

#include "Cameras/PerspectiveCamera.h"

class PerspectiveShadowMapVolume : public FramebufferRenderVolume
{
protected:
	PerspectiveCamera* _lightCamera;

public:
	PerspectiveShadowMapVolume (const Resource<Framebuffer>& framebuffer);
	~PerspectiveShadowMapVolume ();

	void SetLightCamera (PerspectiveCamera* lightCamera);
	void SetShadowBias (float shadowBias);

	PerspectiveCamera* GetLightCamera ();
};

#endif