#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "SceneGraph/SceneObject.h"

#include <vector>
#include <string>

#include "Emiter.h"
#include "Particle.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/ModelView.h"

class ENGINE_API ParticleSystem : public SceneObject
{
protected:
	Emiter* _emiter;
	std::vector<Particle*> _particles;

	std::size_t _emissionRate;
	bool _useDepthMask;
	bool _useGravity;

	std::pair<std::size_t, std::size_t> _partCount;

	float _timeFromLastEmission;

	Resource<ModelView> _modelView;
	unsigned char* _instanceBuffer;
	std::size_t _instanceBufferSize;

public:
	ParticleSystem ();
	~ParticleSystem ();

	void SetEmiter (Emiter* emiter);
	void SetEmissionRate (std::size_t rate);
	void SetMinimPartCount (std::size_t count);
	void SetMaximPartCount (std::size_t count);
	void SetDepthMaskCheck (bool check);
	void SetGravityUse (bool use);

	void Update ();
};

#endif