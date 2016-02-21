#ifndef MESHPARTICLE_H
#define MESHPARTICLE_H

#include "Particle.h"

class MeshParticle : public Particle
{
public:
	MeshParticle ();

	Particle* Clone ();
};

#endif