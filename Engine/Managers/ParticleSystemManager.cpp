#include "ParticleSystemManager.h"

#include <algorithm>

#include "Core/Console/Console.h"

ParticleSystemManager::ParticleSystemManager ()
{

}

ParticleSystemManager::~ParticleSystemManager ()
{

}

void ParticleSystemManager::AddParticleSystem (ParticleSystem* particleSystem)
{
	_particleSystems.push_back (particleSystem);
}

void ParticleSystemManager::RemoveParticleSystem (ParticleSystem* particleSystem)
{
	auto it = std::find (_particleSystems.begin (), _particleSystems.end (), particleSystem);

	if (it == _particleSystems.end ()) {
		return ;
	}

	_particleSystems.erase (it);
}

ParticleSystem* ParticleSystemManager::GetParticleSystem (std::size_t index)
{
	if (0 > index || index >= _particleSystems.size ()) {
		Console::LogWarning ("ParticleSystemManager::elements bounds exceeded");

		return nullptr;
	}

	return _particleSystems [index];
}

std::size_t ParticleSystemManager::GetParticleSystemsCount ()
{
	return _particleSystems.size ();
}