#include "RayPrimitive.h"

RayPrimitive::RayPrimitive (const RayPrimitiveInformation& data) :
	GeometricPrimitive (),
	_data (data)
{

}

RayPrimitive::RayPrimitive (const glm::vec3& origin, const glm::vec3& direction)
{
	_data.origin = origin;
	_data.direction = direction;

	_data.direction = glm::normalize (_data.direction);
}

const RayPrimitive::RayPrimitiveInformation& RayPrimitive::GetVolumeInformation () const
{
	return _data;
}
