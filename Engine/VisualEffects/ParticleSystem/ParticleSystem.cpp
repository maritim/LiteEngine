#include "ParticleSystem.h"

#include <vector>

#include "Systems/Time/Time.h"

#include "Utils/Curves/EaseCurve.h"

#include "Emiter.h"
#include "Particle.h"

#include "ParticleSystemRenderer.h"

ParticleSystem::ParticleSystem () :
	_emiter (nullptr),
	_emissionRate (40),
	_partCount (500, 1000),
	_timeFromLastEmission (0),
	_useDepthMask (false),
	_useGravity (true)
{
	delete _renderer;
	_renderer = new ParticleSystemRenderer (_transform);
	_renderer->SetStageType (Renderer::FORWARD_STAGE);
	_renderer->SetPriority (2);
}

ParticleSystem::~ParticleSystem ()
{
	delete _emiter;

	for (std::size_t i=0;i<_particles.size ();i++) {
		delete _particles [i];
	}
	_particles.clear ();
	_particles.shrink_to_fit ();
}

void ParticleSystem::SetEmiter (Emiter* emiter)
{
	if (_emiter) {
		delete _emiter;
	}

	emiter->GetTransform ()->SetParent (_transform);
	_emiter = emiter;

	ParticleSystemRenderer* renderer = dynamic_cast<ParticleSystemRenderer*> (_renderer);
	renderer->SetInstance (emiter->GetParticlePrototype ());
	renderer->SetDepthMaskCheck (_useDepthMask);
}

void ParticleSystem::SetEmissionRate (std::size_t rate)
{
	_emissionRate = rate;
}

void ParticleSystem::SetMinimPartCount (std::size_t count)
{
	_partCount.first = count;
}

void ParticleSystem::SetDepthMaskCheck (bool check)
{
	_useDepthMask = check;

	ParticleSystemRenderer* renderer = dynamic_cast<ParticleSystemRenderer*> (_renderer);
	renderer->SetDepthMaskCheck (_useDepthMask);
}

void ParticleSystem::SetGravityUse (bool use)
{
	_useGravity = use;
}

void ParticleSystem::SetMaximPartCount (std::size_t count)
{
	_partCount.second = count;

	ParticleSystemRenderer* renderer = dynamic_cast<ParticleSystemRenderer*> (_renderer);
	renderer->SetParticlesCount (count);
}

void ParticleSystem::Update ()
{
	// Update particles
	for (std::size_t i=0;i<_particles.size ();i++) {
		_particles [i]->Update ();
	}

	// Remove dead particles
	for (std::size_t i=0;i<_particles.size ();) {
		if (!_particles [i]->IsAlive ()) {
			Particle* doomed = _particles [i];

			_particles [i] = _particles.back ();
			_particles.pop_back ();

			RemoveParticle (doomed);
		} else {
			++ i;
		}
	}

	// Generate particle at specified rate
	_timeFromLastEmission += Time::GetDeltaTime ();

	float timePerEmission = 1.0 / _emissionRate;

	while ((_timeFromLastEmission > timePerEmission ||
		_particles.size () < _partCount.first) &&
		_particles.size () < _partCount.second) 
	{
		Particle* particle = _emiter->GetParticle ();

		AddParticle (particle);

		if (_timeFromLastEmission > timePerEmission) {
			_timeFromLastEmission -= timePerEmission;
		}
	}
}

void ParticleSystem::AddParticle (Particle* particle)
{
	_particles.push_back (particle);

	ParticleSystemRenderer* renderer = dynamic_cast <ParticleSystemRenderer*> (_renderer);
	renderer->AddRenderer (dynamic_cast<ParticleRenderer*> (particle->GetRenderer ()));
}

void ParticleSystem::RemoveParticle (Particle* particle)
{
	ParticleSystemRenderer* renderer = dynamic_cast <ParticleSystemRenderer*> (_renderer);
	renderer->RemoveRenderer (dynamic_cast<ParticleRenderer*> (particle->GetRenderer ()));

	delete particle;
}