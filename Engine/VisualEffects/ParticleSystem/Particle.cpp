#include "Particle.h"

#include "SceneGraph/SceneObject.h"

#include "Systems/Time/Time.h"

#include "Utils/Extensions/MathExtend.h"

Particle::Particle () :
	SceneObject (),
	_lifetime (0),
	_direction (),
	_speed (0),
	_scaleCurve (NULL),
	_tweenCurve (NULL),
	_alive (true),
	_timeAlive (0),
	_mesh (nullptr)
{

}

Particle::~Particle ()
{

}

void Particle::SetLifetime (unsigned int lifetime)
{
	_lifetime = lifetime;
}

void Particle::SetScaleCurve (AnimationCurve* curve)
{
	_scaleCurve = curve;
}

void Particle::SetTweenCurve (AnimationCurve* curve)
{
	_tweenCurve = curve;
}

void Particle::SetSpeed (float speed)
{
	_speed = speed;
}

void Particle::SetMoveDirection (const glm::vec3& direction)
{
	_direction = direction;
}

void Particle::Init ()
{
	_direction = glm::normalize (_direction);

	_initialPosition = _transform->GetPosition ();
	_initialScale = _transform->GetScale ();
	_finalDestination = _initialPosition + (_direction) * _speed * (_lifetime / 1000.0f);
}

void Particle::SetMesh (const Resource<Model>& mesh)
{
	_mesh = mesh;
}

Resource<Model> Particle::GetMesh () const
{
	return _mesh;
}

bool Particle::IsAlive () const
{
	return _alive;
}

void Particle::Update ()
{
	if (!_alive) {
		return;
	}

	unsigned int udt = Time::GetDeltaTimeMS ();

	_timeAlive += udt;

	if (_timeAlive >= _lifetime) {
		_alive = false;

		return ;
	}

	UpdatePosition ();
	UpdateScale ();
}

void Particle::UpdatePosition ()
{
	float pos = _tweenCurve->Evaluate (1.0f * _timeAlive / _lifetime);

	this->GetTransform ()->SetPosition (
		Extensions::MathExtend::Lerp<glm::vec3> (
			pos, _initialPosition, _finalDestination
		)
	);
}

void Particle::UpdateScale ()
{
	float scale = _scaleCurve->Evaluate (1.0f - 1.0f * _timeAlive / _lifetime);

	this->GetTransform ()->SetScale (_initialScale * scale);
}

void Particle::UpdateRotation ()
{
	// TODO: implement this
}
