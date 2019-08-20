#ifdef _MSC_VER
	#define _USE_MATH_DEFINES
#endif

#include "Emiter.h"

#include <glm/glm.hpp>
#include <cmath>

#include "Core/Math/Matrix.h"
#include "Core/Random/Random.h"

#include "Utils/Primitives/Primitive.h"
#include "Utils/Curves/EaseCurve.h"

#include "BillboardParticle.h"

Emiter::Emiter () :
	_particlePrototype (NULL),
	_scaleCurve (new AnimationCurve (EaseCurve::EaseType::ONE)),
	_tweenCurve (new AnimationCurve ()),
	_lifetime (1000, 2000),
	_speed (1.0f, 1.0f),
	_scale (1.0f, 1.0f),
	_emissionShape (10.0f, 1.0f)
{

}

Emiter::~Emiter ()
{
	delete _particlePrototype;
	delete _scaleCurve;
	delete _tweenCurve;
}

void Emiter::Update ()
{
	// Do nothing
}

void Emiter::SetParticlePrototype (Particle* prototype)
{
	_particlePrototype = prototype;
}

Particle* Emiter::GetParticlePrototype () const
{
	return _particlePrototype;
}

void Emiter::SetEmissionShape (float height, float radius)
{
	_emissionShape.first = height;
	_emissionShape.second = radius;
}

void Emiter::SetScaleCurve (AnimationCurve* scale)
{
	delete _scaleCurve;

	_scaleCurve = scale;
}

void Emiter::SetTweenCurve (AnimationCurve* tween)
{
	delete _tweenCurve;

	_tweenCurve = tween;
}

void Emiter::SetPartLifetimeRange (unsigned int minLifetime, unsigned int maxLifetime)
{
	_lifetime.first = minLifetime;
	_lifetime.second = maxLifetime;
}

void Emiter::SetPartSpeedRange (float minSpeed, float maxSpeed)
{
	_speed.first = minSpeed;
	_speed.second = maxSpeed;
}

void Emiter::SetPartScaleRange (float minScale, float maxScale)
{
	_scale.first = minScale;
	_scale.second = maxScale;
}

Particle* Emiter::InstantiateParticle ()
{
	Particle* clone = _particlePrototype->Clone ();

	return clone;
}

glm::vec3 Emiter::GetParticleDirection (glm::vec3 source)
{
	// Pick direction (see this -> http://mathworld.wolfram.com/DiskPointPicking.html)

	glm::vec3 destPoint;
	float angle = Random::Instance ()->RangeF (0, 2 * M_PI);
	float radius = sqrt (Random::Instance ()->RangeF (0, 1));
	
	destPoint.x = _emissionShape.second * radius * cos (angle);
	destPoint.y = _emissionShape.first;
	destPoint.z = _emissionShape.second * radius * sin (angle);

	// Calculate destination point by emiter rotation
	glm::quat rotQuat = this->GetTransform ()->GetRotation ();
	glm::vec3 rotVec = glm::eulerAngles (rotQuat);

	Matrix rotation = Matrix::Rotate (rotVec.x, rotVec.y, rotVec.z);
	Matrix point (4, 1); point [0][0] = destPoint.x; 
	point [1][0] = destPoint.y; point [2][0] = destPoint.z;

	Matrix resultPoint = rotation * point;

	destPoint.x = resultPoint [0][0]; 
	destPoint.y = resultPoint [1][0]; 
	destPoint.z = resultPoint [2][0];

	// Calculate destination point by emiter translation
	destPoint += this->GetTransform ()->GetPosition ();

	glm::vec3 direction = destPoint - source;

	return direction;
}