#ifndef AABBVOLUME_H
#define AABBVOLUME_H

#include "GeometricPrimitive.h"

#include <glm/glm.hpp>

class AABBVolume : public GeometricPrimitive
{
public:
	struct AABBVolumeInformation
	{
		glm::vec3 minVertex;
		glm::vec3 maxVertex;
	};

private:
	AABBVolumeInformation* _data;

public:
	AABBVolume (AABBVolumeInformation* data);

	AABBVolumeInformation* GetVolumeInformation () const;
};

#endif