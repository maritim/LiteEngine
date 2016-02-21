#ifndef EMITER_H
#define EMITER_H

#include "SceneGraph/SceneObject.h"

#include <vector>

#include "Mesh/Model.h"
#include "Utils/Curves/AnimationCurve.h"

#include "Particle.h"

// TODO: Remade this, maybe with particle prototype ?
class Emiter : public SceneObject
{
private:
	Particle* _particlePrototype;
protected:
	AnimationCurve* _scaleCurve;
	AnimationCurve* _tweenCurve;

	std::pair<unsigned int, unsigned int> _lifetime;
	std::pair<float, float> _speed;
	std::pair<float, float> _scale;
	std::pair<float, float> _emissionShape;

public:
	Emiter ();
	virtual ~Emiter ();

	void SetParticlePrototype (Particle* particle);
	Particle* GetParticlePrototype () const;

	void SetEmissionShape (float height, float radius);

	void SetScaleCurve (AnimationCurve* scale);
	void SetTweenCurve (AnimationCurve* speed);

	void SetPartLifetimeRange (unsigned int minLifetime, unsigned maxLifetime);
	void SetPartSpeedRange (float minSpeed, float maxSpeed);
	void SetPartScaleRange (float minScale, float maxScale);

	void Update ();

	virtual Particle* GetParticle () = 0;
protected:
	Particle* InstantiateParticle ();

	Vector3 GetParticleDirection (Vector3 source);
};

#endif