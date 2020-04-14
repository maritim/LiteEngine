#ifndef AABBVOLUME_H
#define AABBVOLUME_H

#include <limits>
#include <glm/vec3.hpp>

struct ENGINE_API AABBVolume
{
	glm::vec3 minVertex;
	glm::vec3 maxVertex;

	AABBVolume () :
		minVertex (std::numeric_limits<float>::infinity()),
		maxVertex (-std::numeric_limits<float>::infinity())
	{

	}

	AABBVolume (const glm::vec3& minVertex1, const glm::vec3& maxVertex1) :
		minVertex (minVertex1),
		maxVertex (maxVertex1)
	{

	}
};

#endif