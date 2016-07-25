#ifndef SPHEREEMITER_H
#define SPHEREEMITER_H

#include "PrimitiveEmiter.h"

#include "Core/Math/glm/vec3.hpp"

#include "Particle.h"


class SphereEmiter : public PrimitiveEmiter
{
protected:
	float _radius;

public:
	SphereEmiter (float radius = 1.0f);

	void SetRadius (float radius);
protected:
	glm::vec3 GetParticlePosition ();
};

#endif