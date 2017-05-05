#ifndef LIGHTSHADOWMAPRENDERER_H
#define LIGHTSHADOWMAPRENDERER_H

#include "Lighting/LightRenderer.h"

#include "ShadowMapVolume.h"

class LightShadowMapRenderer : public LightRenderer
{
protected:
	ShadowMapVolume* _volume;

public:
	LightShadowMapRenderer (Light* light);
	~LightShadowMapRenderer ();

	void Draw (Scene* scene, Camera* camera);
protected:
	virtual void ShadowMapPass (Scene* scene, Camera* camera);
	virtual void RenderingPass (Scene* scene, Camera* camera);

	virtual void ShadowMapRender (Scene* scene, Camera* camera);
};

#endif