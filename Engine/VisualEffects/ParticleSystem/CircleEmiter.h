#ifndef CIRCLEEMITER_H
#define CIRCLEEMITER_H

#include "Emiter.h"
#include "PrimitiveEmiter.h"

#include <glm/glm.hpp>

#include "Particle.h"


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