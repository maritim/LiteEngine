#ifndef PHYSICS_H
#define PHYSICS_H

#include "Core/Math/Vector3.h"

class Physics
{
private:
	float _g;

public:
	static Physics& Instance ();

	float GetG () const;
	Vector3 GetGravityVector () const;

private:
	Physics ();
};

#endif