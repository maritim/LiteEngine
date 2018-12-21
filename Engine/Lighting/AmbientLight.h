#ifndef AMBIENT_LIGHT
#define AMBIENT_LIGHT

#include "VolumetricLight.h"

class AmbientLight : public Light
{
public:
	AmbientLight ();

	void Update ();

	void OnAttachedToScene ();
};

#endif