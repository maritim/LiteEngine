#ifndef CIRCLEEMITER_H
#define CIRCLEEMITER_H

#include "Emiter.h"
#include "PrimitiveEmiter.h"

#include "Particle.h"

#include "Core/Math/glm/glm.hpp"

class CircleEmiter : public PrimitiveEmiter
{
protected:
	float _radius;

public:
	CircleEmiter (float radius = 1.0f);

	void SetRadius (float radius);
private:
	glm::vec3 GetParticlePosition ();
};

#endif