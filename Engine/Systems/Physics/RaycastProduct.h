#ifndef RAYCASTPRODUCT_H
#define RAYCASTPRODUCT_H

#include <glm/vec3.hpp>

struct RaycastProduct
{
	bool isCollision;

	glm::vec3 hitPosition;

	glm::vec3 hitNormal;

	RaycastProduct () :
		isCollision (false),
		hitPosition (0.0f),
		hitNormal (0.0f)
	{

	}
};

#endif