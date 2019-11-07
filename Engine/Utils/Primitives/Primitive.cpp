#include "Primitive.h"

#include <vector>
#include <string>

#include "Resources/Resources.h"

Primitive::Primitive ()
{
	const static std::size_t primitivesCount = 7;
	const static std::string primitive_filenames[] = {
		"Assets/Primitives/Quad.obj",
		"Assets/Primitives/Cube.obj",
		"Assets/Primitives/Sphere.obj",
		"Assets/Primitives/Triangular_Pyramid.obj",
		"Assets/Primitives/Square_Pyramid.obj",
		"Assets/Primitives/Cone.obj",
		"Assets/Primitives/Cylinder.obj",
	};

	for (std::size_t i=0;i<primitivesCount;i++) {
		Resource<Model> primitive = Resources::LoadModel (primitive_filenames [i]);

		_primitives.push_back (primitive);
	}
}

Primitive::~Primitive ()
{

}

Resource<Model> Primitive::Create (Type type)
{
	return _primitives [type];
}