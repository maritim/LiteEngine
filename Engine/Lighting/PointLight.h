#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "VolumetricLight.h"

class PointLight : public VolumetricLight
{
protected:
	float _range;

public:
	PointLight ();

	float GetRange () const;

	void SetRange (float range);

	void Update ();

	void OnAttachedToScene ();
	void OnDetachedFromScene ();
protected:
	virtual void UpdateTransform ();
};

#endif