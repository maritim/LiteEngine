#include "PrimitiveEmiter.h"

#include "Core/Random/Random.h"

PrimitiveEmiter::PrimitiveEmiter () :
	Emiter ()
{

}

Particle* PrimitiveEmiter::GetParticle ()
{
	unsigned int lifetime = Random::Instance ()->RangeI (_lifetime.first, _lifetime.second);
	float speed = Random::Instance ()->RangeF (_speed.first, _speed.second);
	float scale = Random::Instance ()->RangeF (_scale.first, _scale.second);

	glm::vec3 position = this->GetParticlePosition ();
	glm::vec3 direction = this->GetParticleDirection (position);

	Particle* particle = InstantiateParticle ();

	particle->SetLifetime (lifetime);
	particle->SetSpeed (speed);
	particle->SetScaleCurve (_scaleCurve);
	particle->SetTweenCurve (_tweenCurve);
	particle->SetMoveDirection (new glm::vec3 (direction));
	particle->GetTransform ()->SetPosition (position);
	particle->GetTransform ()->SetScale (glm::vec3 (1.0f) * scale);
	particle->GetTransform ()->SetRotation (this->GetTransform ()->GetRotation ());

	particle->Init ();

	return particle;
}

glm::vec3 PrimitiveEmiter::GetParticlePosition ()
{
	return this->GetTransform ()->GetPosition ();
}