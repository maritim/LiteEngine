#ifndef RSMDIRECTIONALLIGHTVOLUME_H
#define RSMDIRECTIONALLIGHTVOLUME_H

#include "RenderPasses/FramebufferRenderVolume.h"

#include "Systems/Camera/Camera.h"

#define REFLECTIVE_SHADOW_MAP_FBO_NOT_INIT 350

class RSMVolume : public FramebufferRenderVolume
{
protected:
	Camera* _camera;

public:
	RSMVolume (const Resource<Framebuffer>& framebuffer);
	~RSMVolume ();

	void SetLightCamera (Camera* camera);
	void SetShadowBias (float shadowBias);

	Camera* GetLightCamera ();
};

#endif