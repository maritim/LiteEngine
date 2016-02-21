#ifndef PRIMITIVEEMITER_H
#define PRIMITIVEEMITER_H

#include "Emiter.h"

#include "Core/Math/Vector3.h"

#include "Particle.h"

class PrimitiveEmiter : public Emiter
{
public:
	PrimitiveEmiter ();

	Particle* GetParticle ();
protected:
	virtual Vector3 GetParticlePosition ();
};

#endif