#ifndef SPHEREEMITER_H
#define SPHEREEMITER_H

#include "PrimitiveEmiter.h"

#include "Core/Math/Vector3.h"

#include "Particle.h"


class SphereEmiter : public PrimitiveEmiter
{
protected:
	float _radius;

public:
	SphereEmiter (float radius = 1.0f);

	void SetRadius (float radius);
protected:
	Vector3 GetParticlePosition ();
};

#endif