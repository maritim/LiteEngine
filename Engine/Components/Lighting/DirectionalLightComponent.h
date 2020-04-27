#ifndef DIRECTIONALLIGHTCOMPONENT_H
#define DIRECTIONALLIGHTCOMPONENT_H

#include "LightComponent.h"

class ENGINE_API DirectionalLightComponent : public LightComponent
{
	DECLARE_COMPONENT(DirectionalLightComponent)

public:
	void Awake ();

	void OnAttachedToScene ();
	void OnDetachedFromScene ();
};

#endif