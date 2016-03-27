#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Core/Singleton/Singleton.h"

#include <vector>

#include "Mesh/Model.h"

class Primitive : public Singleton<Primitive>
{
	friend class Singleton<Primitive>;

public:
	enum Type { QUAD = 0, CUBE, SPHERE, TRIANGULAR_PYRAMID, SQUARE_PYRAMID };

private:
	std::vector<Model*> _primitives;

public:
	Model* Create (Type type);
private:
	Primitive ();
	Primitive (const Primitive& other);
	Primitive& operator=(const Primitive& other);
	~Primitive ();
};

#endif