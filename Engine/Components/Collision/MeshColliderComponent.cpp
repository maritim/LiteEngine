#include "MeshColliderComponent.h"

#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <glm/gtc/matrix_transform.hpp>

MeshColliderComponent::MeshColliderComponent () :
	_triangleMesh (nullptr)
{

}

MeshColliderComponent::~MeshColliderComponent ()
{
	delete _triangleMesh;
}

void MeshColliderComponent::Awake ()
{
	if (_model == nullptr) {
		return;
	}

	SetModel (_model);
}

void MeshColliderComponent::SetModel (const Resource<Model>& model)
{
	_model = model;

	OnDetachedFromScene ();

	/*
	 * Destroy current collision shape if exists
	*/

	delete _collisionShape;

	/*
	 * Destroy current triangle mesh if exists
	*/

	delete _triangleMesh;

	/*
	 * Compute collision triangle mesh based on object's mesh
	*/

	_triangleMesh = GetTriangleMesh (model);

	_collisionShape = new btBvhTriangleMeshShape (_triangleMesh, true, true);

	OnAttachedToScene ();
}

btTriangleMesh* MeshColliderComponent::GetTriangleMesh (const Resource<Model>& model)
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
