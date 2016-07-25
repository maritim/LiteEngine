#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "VolumetricLight.h"

class SpotLight : public VolumetricLight
{
protected:
	float _constantAttenuation, _linearAttenuation, _quadraticAttenuation;
	float _spotCutoff, _spotExponent;
	glm::vec3 _spotDirection;

public:
	SpotLight ();

	void Update ();

	glm::vec3 GetSpotDirection () const;
	float GetSpotCutoff () const;
	float GetSpotExponent () const;

	void SetSpotDirection (const glm::vec3& spotDirection);
	void SetSpotCutoff (float spotCutoff);
	void SetSpotExponent (float spotExponent);

	void OnAttachedToScene ();
	void OnDetachedFromScene ();
};

#endif