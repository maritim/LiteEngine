#ifndef RAYPRIMITIVE_H
#define RAYPRIMITIVE_H

#include "GeometricPrimitive.h"

#include <glm/glm.hpp>

class RayPrimitive : public GeometricPrimitive
{
public:
	struct RayPrimitiveInformation
	{
		glm::vec3 origin;
		glm::vec3 direction;
	};

private:
	RayPrimitiveInformation _data;

public:
	RayPrimitive (const RayPrimitiveInformation& data);
	RayPrimitive (const glm::vec3& origin, const glm::vec3& direction);

	const RayPrimitiveInformation& GetVolumeInformation () const;
};

#endif