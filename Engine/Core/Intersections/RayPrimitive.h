#ifndef RAYPRIMITIVE_H
#define RAYPRIMITIVE_H

#include <glm/vec3.hpp>

struct ENGINE_API RayPrimitive
{
	glm::vec3 origin;
	glm::vec3 direction;

	RayPrimitive (const glm::vec3& origin1, const glm::vec3& direction1) :
		origin (origin1),
		direction (direction1)
	{

	}
};

#endif