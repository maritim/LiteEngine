#ifndef BILLBOARDPARTICLE_H
#define BILLBOARDPARTICLE_H

#include "Particle.h"

class BillboardParticle : public Particle
{
public:
	BillboardParticle ();

	Particle* Clone ();
};

#endif