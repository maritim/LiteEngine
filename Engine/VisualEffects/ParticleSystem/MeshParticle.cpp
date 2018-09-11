#include "MeshParticle.h"

#include "Particle.h"

#include "MeshParticleRenderer.h"

MeshParticle::MeshParticle () :
	Particle ()
{
	delete _renderer;
	_renderer = new MeshParticleRenderer (_transform);
}

Particle* MeshParticle::Clone ()
{
	MeshParticle* clone = new MeshParticle ();

	return clone;
}