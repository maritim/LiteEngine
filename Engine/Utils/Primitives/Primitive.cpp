#include "Primitive.h"

#include <vector>
#include <string>

#include "Resources/Resources.h"

Primitive::Primitive ()
{
	const static std::size_t primitivesCount = 5;
	const static std::string primitive_filenames[] = {
		"Assets/Primitives/Quad.obj",
		"Assets/Primitives/Cube.obj",
		"Assets/Primitives/Sphere.obj",
		"Assets/Primitives/Triangular_Pyramid.obj",
		"Assets/Primitives/Square_Pyramid.obj",
	};

	for (std::size_t i=0;i<primitivesCount;i++) {
		Model* primitive = Resources::LoadModel (primitive_filenames [i]);

		_primitives.push_back (primitive);
	}
}

Primitive::~Primitive ()
{

}

Primitive& Primitive::Instance ()
{
	static Primitive primitive;

	return primitive;
}

Model* Primitive::Create (Type type)
{
	Model* primitive = new Model (*_primitives [type]);

	return primitive;
}