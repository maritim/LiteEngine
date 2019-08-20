#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "VolumetricLight.h"

#include <glm/vec3.hpp>

class PointLight : public VolumetricLight
{
protected:
	glm::vec3 _attenuation;

public:
	PointLight ();

	void SetAttenuation (const glm::vec3& attenuation);

	void SetActive (bool isActive);

	void Update ();

	void OnAttachedToScene ();
	void OnDetachedFromScene ();
protected:
	void UpdateScale ();
};

#endif