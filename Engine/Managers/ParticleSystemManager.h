#ifndef PARTICLESYSTEMMANAGER_H
#define PARTICLESYSTEMMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <vector>

#include "VisualEffects/ParticleSystem/ParticleSystem.h"

class ParticleSystemManager : public Singleton<ParticleSystemManager>
{
	friend Singleton<ParticleSystemManager>;

private:
	std::vector<ParticleSystem*> _particleSystems;

public:
	void AddParticleSystem (ParticleSystem* particleSystem);
	void RemoveParticleSystem (ParticleSystem* particleSystem);

	ParticleSystem* GetParticleSystem (std::size_t index);
	std::size_t GetParticleSystemsCount ();
private:
	ParticleSystemManager ();
	~ParticleSystemManager ();
	ParticleSystemManager (const ParticleSystemManager&);
	ParticleSystemManager& operator=(const ParticleSystemManager&);
};

#endif