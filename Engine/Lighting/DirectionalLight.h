#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT

#include "VolumetricLight.h"

class DirectionalLight : public VolumetricLight
{
public:
	DirectionalLight ();

	void Update ();

	void OnAttachedToScene ();
	void OnDetachedFromScene ();
};

#endif