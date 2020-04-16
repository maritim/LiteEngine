#ifndef POINTLIGHTCOMPONENT_H
#define POINTLIGHTCOMPONENT_H

#include "LightComponent.h"

class ENGINE_API PointLightComponent : public LightComponent
{
	DECLARE_COMPONENT(PointLightComponent)

protected:
	ATTRIBUTE(EditAnywhere, Meta)	
	float _range;

public:
	void Awake ();

	void Update ();

	void OnAttachedToScene ();
	void OnDetachedFromScene ();

	float GetRange () const;

	void SetRange (float range);
};

#endif