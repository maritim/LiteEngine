#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <vector>

#include "Mesh/Model.h"

class Primitive
{
public:
	enum Type { QUAD = 0, CUBE, SPHERE, TRIANGULAR_PYRAMID, SQUARE_PYRAMID };

private:
	std::vector<Model*> _primitives;

public:
	~Primitive ();

	static Primitive& Instance ();	

	Model* Create (Type type);
private:
	Primitive ();
};

#endif