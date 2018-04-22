#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/vec3.hpp>

class Physics
{
private:
	float _g;

public:
	static Physics& Instance ();

	float GetG () const;
	glm::vec3 GetGravityVector () const;

private:
	Physics ();
};

#endif