#include "CircleEmiter.h"

#include <cmath>

#include "Particle.h"

#include "Core/Math/Matrix.h"
#include "Core/Random/Random.h"

CircleEmiter::CircleEmiter (float radius) :
	_radius (radius)
{

}

void CircleEmiter::SetRadius (float radius)
{
	_radius = radius;
}

Vector3 CircleEmiter::GetParticlePosition ()
{
	Vector3 position;

	float radius = sqrt (Random::Instance ().RangeF (0, 1.0f));
	float angle = Random::Instance ().RangeF (0, 2 * M_PI);

	position.x = _radius * radius * cos (angle);
	position.z = _radius * radius * sin (angle);

	// Calculate by rotation
	Vector3 rotVec = this->GetTransform ()->GetRotation ();
	Matrix rotation = Matrix::Rotate (rotVec.x, rotVec.y, rotVec.z);
	Matrix point (4, 1); point [0][0] = position.x; point [2][0] = position.z;

	Matrix resultPoint = rotation * point;

	position.x = resultPoint [0][0]; 
	position.y = resultPoint [1][0]; 
	position.z = resultPoint [2][0];

	// Calculate by translation
	position += this->GetTransform ()->GetPosition ();

	return position;
}