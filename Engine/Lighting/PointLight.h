#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "VolumetricLight.h"

class PointLight : public VolumetricLight
{
protected:
	float _constantAttenuation, _linearAttenuation, _quadraticAttenuation;

public:
	PointLight ();

	void Update ();

	float GetConstantAttenuation () const;
	float GetLinearAttenuation () const;
	float GetQuadraticAttenuation () const;

	void SetConstantAttenuation (float constantAttenuation);
	void SetLinearAttenuation (float linearAttenuation);
	void SetQuadraticAttenuation (float quadraticAttenuation);

	void OnAttachedToScene ();
	void OnDetachedFromScene ();

};

#endif