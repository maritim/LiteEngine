#include "MeshCollider.h"

#include <bullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <glm/gtc/matrix_transform.hpp>

MeshCollider::MeshCollider (const Resource<Model>& model) :
	_triangleMesh (nullptr)
{
	Rebuild (model);
}

MeshCollider::~MeshCollider ()
{
	DestroyTriangleMesh ();
}

void MeshCollider::Rebuild (const Resource<Model>& model)
{
	/*
	 * Destroy current collision shape if exists
	*/

	DestroyCollisionShape ();

	/*
	 * Destroy current triangle mesh if exists
	*/

	DestroyTriangleMesh ();

	/*
	 * Compute collision triangle mesh based on object's mesh
	*/

	_triangleMesh = GetTriangleMesh (model);

	_collisionShape = new btBvhTriangleMeshShape (_triangleMesh, true, true);
}

btTriangleMesh* MeshCollider::GetTriangleMesh (const Resource<Model>& model)
{
	btTriangleMesh* triangleMesh = new btTriangleMesh ();

	/*
	 * Iterate over all vertices and create triangle mesh collider
	*/

	for_each_type (ObjectModel*, objModel, *model) {
		for (PolygonGroup* polyGroup : *objModel) {
			for (Polygon* polygon : *polyGroup) {
				btVector3 vertices [3];

				for(std::size_t vertexIndex=0;vertexIndex<polygon->VertexCount();vertexIndex++) {
					glm::vec3 vertex = model->GetVertex (polygon->GetVertex (vertexIndex));

					vertices [vertexIndex].setX (vertex.x);
					vertices [vertexIndex].setY (vertex.y);
					vertices [vertexIndex].setZ (vertex.z);
				}

				triangleMesh->addTriangle (vertices [0], vertices [1], vertices [2]);
			}
		}
	}

	return triangleMesh;
}

void MeshCollider::DestroyTriangleMesh ()
{
	if (_triangleMesh == nullptr) {
		return;
	}

	delete _triangleMesh;
}