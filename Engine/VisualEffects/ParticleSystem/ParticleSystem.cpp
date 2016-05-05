#include "ParticleSystem.h"

#include <vector>

#include "Systems/Time/Time.h"

#include "Utils/Curves/EaseCurve.h"

#include "Emiter.h"
#include "Particle.h"

#include "ParticleSystemRenderer.h"

#include "Managers/ParticleSystemManager.h"

ParticleSystem::ParticleSystem () :
	_emiter (nullptr),
	_emissionRate (40),
	_partCount (500, 1000),
	_timeFromLastEmission (0),
	_useDepthMask (false),
	_useGravity (true),
	_particleSystemRenderer (nullptr)
{
	_particleSystemRenderer = new ParticleSystemRenderer (_transform);
	_particleSystemRenderer->SetPriority (2);
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

	_particleSystemRenderer->SetInstance (emiter->GetParticlePrototype ());
	_particleSystemRenderer->SetDepthMaskCheck (_useDepthMask);
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

	_particleSystemRenderer->SetDepthMaskCheck (_useDepthMask);
}

void ParticleSystem::SetGravityUse (bool use)
{
	_useGravity = use;
}

void ParticleSystem::SetMaximPartCount (std::size_t count)
{
	_partCount.second = count;

	_particleSystemRenderer->SetParticlesCount (count);
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

ParticleSystemRenderer* ParticleSystem::GetParticleSystemRenderer ()
{
	return _particleSystemRenderer;
}

void ParticleSystem::OnAttachedToScene ()
{
	ParticleSystemManager::Instance ()->AddParticleSystem (this);
}

void ParticleSystem::OnDetachedFromScene ()
{
	ParticleSystemManager::Instance ()->RemoveParticleSystem (this);
}

void ParticleSystem::AddParticle (Particle* particle)
{
	_particles.push_back (particle);

	_particleSystemRenderer->AddRenderer (dynamic_cast<ParticleRenderer*> (particle->GetRenderer ()));
}

void ParticleSystem::RemoveParticle (Particle* particle)
{
	_particleSystemRenderer->RemoveRenderer (dynamic_cast<ParticleRenderer*> (particle->GetRenderer ()));

	delete particle;
}