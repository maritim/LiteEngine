#ifndef PRIMITIVEEMITER_H
#define PRIMITIVEEMITER_H

#include "Emiter.h"

#include <glm/glm.hpp>

#include "Particle.h"

class PrimitiveEmiter : public Emiter
{
public:
	PrimitiveEmiter ();

	Particle* GetParticle ();
protected:
	virtual glm::vec3 GetParticlePosition ();
};

#endif