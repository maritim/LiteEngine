#ifndef CIRCLEEMITER_H
#define CIRCLEEMITER_H

#include "Emiter.h"
#include "PrimitiveEmiter.h"

#include "Particle.h"

#include "Core/Math/Vector3.h"

class CircleEmiter : public PrimitiveEmiter
{
protected:
	float _radius;

public:
	CircleEmiter (float radius = 1.0f);

	void SetRadius (float radius);
private:
	Vector3 GetParticlePosition ();
};

#endif