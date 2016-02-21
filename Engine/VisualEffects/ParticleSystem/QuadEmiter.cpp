#include "QuadEmiter.h"

#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"

#include "Core/Random/Random.h"

QuadEmiter::QuadEmiter (float width, float depth) :
	PrimitiveEmiter (),
	_width (width),
	_depth (depth)
{

}

void QuadEmiter::SetWidth (float width)
{
	_width = width;
}

void QuadEmiter::SetDepth (float depth)
{
	_depth = depth;
}

Vector3 QuadEmiter::GetParticlePosition ()
{
	Vector3 position;

	position.x = Random::Instance ().RangeF (-_width / 2, _width / 2);
	position.z = Random::Instance ().RangeF (-_depth / 2, _depth / 2);

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