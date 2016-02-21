/*
 * The emiter need to choose a random position inside a three-dimensional
 * mesh, but instead of this, I did a trick. I choosed random points inside
 * triangles of the mesh. For the purpose of the emiter is the same thing.
 * To do this, I research the web and find this solution:
 * http://math.stackexchange.com/questions/538458/triangle-point-picking-in-3d
 * which is uber nice, in my opinion.
 *
 * I will explain here too:
 * 			
 *			x = v1 + a (v2 - v1) + b (v3 - v1)
 *
 * where x is the resulted position, v1, v2, v3 are the points of the
 * triangle and a, b are random interpolating values which a + b = 1
 *
 *
 */

#include "MeshEmiter.h"

#include "Core/Random/Random.h"

MeshEmiter::MeshEmiter (Model* mesh) :
	_mesh (mesh)
{
	for (std::size_t i=0;i<mesh->ObjectsCount ();i++) {
		ObjectModel* objModel = mesh->GetObject (i);

		ProcessObjectModel (mesh, objModel);
	}
}

MeshEmiter::~MeshEmiter ()
{
	_meshSamples.clear ();
	_meshSamples.shrink_to_fit ();

	delete _mesh;
}

Vector3 MeshEmiter::GetParticlePosition ()
{
	std::size_t pos = Random::Instance ().RangeI (0, _meshSamples.size () - 1);
	float t = Random::Instance ().RangeF (0, 1);

	MeshSample sample = _meshSamples [pos];

	Vector3 position = (*sample.a) + (*sample.b - *sample.a) * t +
		(*sample.c - *sample.a) * (1.0 - t);

	position += _transform->GetPosition ();

	return position;
}

void MeshEmiter::ProcessObjectModel (Model* mesh, ObjectModel* objModel)
{
	for (std::size_t i=0;i<objModel->GetPolygonCount ();i++) {
		PolygonGroup* polyGroup = objModel->GetPolygonGroup (i);

		ProcessPolygonGroup (mesh, polyGroup);
	}
}

void MeshEmiter::ProcessPolygonGroup (Model* mesh, PolygonGroup* polyGroup)
{
	for (std::size_t i=0;i<polyGroup->GetPolygonCount ();i++) {
		Polygon* polygon = polyGroup->GetPolygon (i);

		MeshSample sample;
		sample.a = mesh->GetVertex (polygon->GetVertex (0));
		sample.b = mesh->GetVertex (polygon->GetVertex (1));
		sample.c = mesh->GetVertex (polygon->GetVertex (2));

		_meshSamples.push_back (sample);
	}
}