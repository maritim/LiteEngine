#include "ParticleSystem.h"

#include <vector>
#include <cstring>

#include "Systems/Time/Time.h"

#include "Utils/Curves/EaseCurve.h"

#include "Emiter.h"
#include "Particle.h"

#include "Renderer/RenderSystem.h"

ParticleSystem::ParticleSystem () :
	_emiter (nullptr),
	_emissionRate (40),
	_useDepthMask (false),
	_useGravity (true),
	_partCount (500, 1000),
	_timeFromLastEmission (0),
	_modelView (nullptr),
	_instanceBuffer (nullptr),
	_instanceBufferSize (0)
{

}

ParticleSystem::~ParticleSystem ()
{
	delete _emiter;

	for (std::size_t i=0;i<_particles.size ();i++) {
		delete _particles [i];
	}
	_particles.clear ();
	_particles.shrink_to_fit ();

	delete[] _instanceBuffer;
}

void ParticleSystem::SetEmiter (Emiter* emiter)
{
	if (_emiter) {
		delete _emiter;
	}

	emiter->GetTransform ()->SetParent (_transform);
	_emiter = emiter;

	// AttachMesh (emiter->GetParticlePrototype ()->GetMesh ());

	_modelView = RenderSystem::LoadModel (emiter->GetParticlePrototype ()->GetMesh ());

	// _renderObject->SetTransform (_transform);
	// _renderObject->SetModelView (_modelView);
	// _renderObject->SetRenderStage (RenderStage::RENDER_STAGE_FORWARD);
	// _renderObject->SetPriority (2);
	// _renderObject->SetAttributes (emiter->GetParticlePrototype ()->GetAttributes ());
	// renderer->SetDepthMaskCheck (_useDepthMask);
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

	// ParticleSystemRenderer* renderer = dynamic_cast<ParticleSystemRenderer*> (_renderer);
	// renderer->SetDepthMaskCheck (_useDepthMask);
}

void ParticleSystem::SetGravityUse (bool use)
{
	_useGravity = use;
}

void ParticleSystem::SetMaximPartCount (std::size_t count)
{
	_partCount.second = count;
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

			delete doomed;
		} else {
			++ i;
		}
	}

	// Generate particle at specified rate
	_timeFromLastEmission += Time::GetDeltaTime ();

	float timePerEmission = 1.0f / _emissionRate;

	while ((_timeFromLastEmission > timePerEmission ||
		_particles.size () < _partCount.first) &&
		_particles.size () < _partCount.second) 
	{
		Particle* particle = _emiter->GetParticle ();

		_particles.push_back (particle);

		if (_timeFromLastEmission > timePerEmission) {
			_timeFromLastEmission -= timePerEmission;
		}
	}

	if (_instanceBuffer == nullptr) {
		auto particlePrototype = _emiter->GetParticlePrototype ();
		_instanceBufferSize = particlePrototype->GetSize () * _partCount.second;
		_instanceBuffer = new unsigned char [_instanceBufferSize];

		RenderSystem::CreateInstanceModelView (_modelView, particlePrototype->GetBufferAttributes (), _instanceBufferSize);
	}

	std::size_t bufferIndex = 0;
	for (auto particle : _particles) {
		std::memcpy (_instanceBuffer + bufferIndex, particle->GetBuffer (), particle->GetSize ());

		bufferIndex += particle->GetSize ();
	}

	RenderSystem::UpdateInstanceModelView (_modelView, _instanceBufferSize, _particles.size (), _instanceBuffer);
}
