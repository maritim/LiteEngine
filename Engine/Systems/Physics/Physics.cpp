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

Vector3 Physics::GetGravityVector () const
{
	return Vector3::Down * _g;
}
