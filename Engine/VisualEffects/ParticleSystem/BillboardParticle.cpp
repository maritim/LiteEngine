#include "BillboardParticle.h"

#include "Particle.h"

#include "BillboardParticleRenderer.h"

BillboardParticle::BillboardParticle () :
	Particle ()
{
	delete _renderer;

	BillboardParticleRenderer* billboardRenderer = 
		new BillboardParticleRenderer (_transform);

	billboardRenderer->SetLifetime (&_lifetime);
	billboardRenderer->SetTimeAlive (&_timeAlive);

	_renderer = billboardRenderer;	
}

Particle* BillboardParticle::Clone ()
{
	BillboardParticle* clone = new BillboardParticle ();

	clone->SetMesh (_mesh);

	return clone;
}