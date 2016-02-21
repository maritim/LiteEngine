#include "PrimitiveEmiter.h"

#include "Core/Random/Random.h"

PrimitiveEmiter::PrimitiveEmiter () :
	Emiter ()
{

}

Particle* PrimitiveEmiter::GetParticle ()
{
	unsigned int lifetime = Random::Instance ().RangeI (_lifetime.first, _lifetime.second);
	float speed = Random::Instance ().RangeF (_speed.first, _speed.second);
	float scale = Random::Instance ().RangeF (_scale.first, _scale.second);

	Vector3 position = this->GetParticlePosition ();
	Vector3 direction = this->GetParticleDirection (position);

	Particle* particle = InstantiateParticle ();

	particle->SetLifetime (lifetime);
	particle->SetSpeed (speed);
	particle->SetScaleCurve (_scaleCurve);
	particle->SetTweenCurve (_tweenCurve);
	particle->SetMoveDirection (new Vector3 (direction));
	particle->GetTransform ()->SetPosition (position);
	particle->GetTransform ()->SetScale (Vector3::One * scale);
	particle->GetTransform ()->SetRotation (this->GetTransform ()->GetRotation ());

	particle->Init ();

	return particle;
}

Vector3 PrimitiveEmiter::GetParticlePosition ()
{
	return this->GetTransform ()->GetPosition ();
}