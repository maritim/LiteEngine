#include "VectorExtend.h"

#include <glm/glm.hpp>

using namespace Extensions;

glm::vec3 VectorExtend::Cross (const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
{
	/*
	 * Compute U, V
	*/

	glm::vec3 u = p1 - p0;
	glm::vec3 v = p2 - p0;

	/*
	 * Compute cross product
	*/

	glm::vec3 normal = glm::cross (u, v);

	return normal;
}