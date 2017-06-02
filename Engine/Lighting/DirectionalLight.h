#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT

#include "VolumetricLight.h"

class DirectionalLight : public VolumetricLight
{
public:
	DirectionalLight ();

	void SetShadowCasting (bool casting);
	void SetRenderMode (LightRenderMode lightMode);

	void Update ();

	void OnAttachedToScene ();
	void OnDetachedFromScene ();
protected:
	std::vector<PipelineAttribute> GetCustomAttributes ();

	void ChangeShadowCasting (bool casting);
};

#endif