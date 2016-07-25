#include "Physics.h"

Physics::Physics () :
	_g (2)
{

}

Physics& Physics::Instance ()
{
	static Physics instance;

	return instance;
}

float Physics::GetG () const
{
	return _g;
}

glm::vec3 Physics::GetGravityVector () const
{
	return glm::vec3 (0.0f, -1.0f, 0.0f) * _g;
}
