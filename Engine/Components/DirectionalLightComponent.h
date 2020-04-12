#ifndef DIRECTIONALLIGHTCOMPONENT_H
#define DIRECTIONALLIGHTCOMPONENT_H

#include "LightComponent.h"

class ENGINE_API DirectionalLightComponent : public LightComponent
{
public:
	void Awake ();

	void OnAttachedToScene ();
	void OnDetachedFromScene ();
};

#endif