#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "SceneGraph/SceneObject.h"

#include <vector>
#include <string>

#include "Emiter.h"
#include "Particle.h"

#include "ParticleSystemRenderer.h"

class ParticleSystem : public SceneObject
{
protected:
	Emiter* _emiter;
	std::vector<Particle*> _particles;

	std::size_t _emissionRate;
	bool _useDepthMask;
	bool _useGravity;

	std::pair<std::size_t, std::size_t> _partCount;

	float _timeFromLastEmission;

	ParticleSystemRenderer* _particleSystemRenderer;

public:
	ParticleSystem ();
	~ParticleSystem ();

	void SetEmiter (Emiter* emiter);
	void SetEmissionRate (std::size_t rate);
	void SetMinimPartCount (std::size_t count);
	void SetMaximPartCount (std::size_t count);
	void SetDepthMaskCheck (bool check);
	void SetGravityUse (bool use);

	ParticleSystemRenderer* GetParticleSystemRenderer ();

	void Update ();

	void OnAttachedToScene ();
	void OnDetachedFromScene ();
private:
	void AddParticle (Particle* particle);
	void RemoveParticle (Particle* particle);
};

#endif