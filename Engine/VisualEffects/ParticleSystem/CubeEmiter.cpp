#include "CubeEmiter.h"

#include "Core/Math/Matrix.h"

#include "Core/Random/Random.h"

CubeEmiter::CubeEmiter (float width, float depth, float height) :
	QuadEmiter (width, depth),
	_height (height)
{

}

void CubeEmiter::SetHeight (float height)
{
	_height = height;
}

glm::vec3 CubeEmiter::GetParticlePosition ()
{
	glm::vec3 position;

	position.x = Random::Instance ().RangeF (-_width / 2, _width / 2);
	position.y = Random::Instance ().RangeF (-_height / 2, _height / 2);
	position.z = Random::Instance ().RangeF (-_depth / 2, _depth / 2);

	// Calculate by rotation
	glm::quat rotQuat = this->GetTransform ()->GetRotation ();
	glm::vec3 rotVec = glm::eulerAngles (rotQuat);
	Matrix rotation = Matrix::Rotate (rotVec.x, rotVec.y, rotVec.z);
	Matrix point (4, 1); point [0][0] = position.x; 
	point [1][0] = position.y; point [2][0] = position.z;

	Matrix resultPoint = rotation * point;

	position.x = resultPoint [0][0]; 
	position.y = resultPoint [1][0]; 
	position.z = resultPoint [2][0];

	// Calculate by translation
	position += this->GetTransform ()->GetPosition ();

	return position;
}