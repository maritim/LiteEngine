#include "MeshCollider.h"

#include <bullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <glm/gtc/matrix_transform.hpp>

MeshCollider::~MeshCollider ()
{
	DestroyTriangleMesh ();
}

void MeshCollider::Rebuild (Model* mesh, Transform* transform)
{
	/*
	 * Destroy current triangle mesh if exists
	*/

	DestroyTriangleMesh ();

	/*
	 * Compute collision triangle mesh based on object's mesh
	*/

	_triangleMesh = GetTriangleMesh (mesh, transform);


	/*
	 * Destroy current collision shape if exists
	*/

	DestroyCollisionShape ();

	_collisionShape = new btBvhTriangleMeshShape (_triangleMesh, true, true);
}

btTriangleMesh* MeshCollider::GetTriangleMesh (Model* mesh, Transform* transform)
{
	btTriangleMesh* triangleMesh = new btTriangleMesh ();

	/*
	 * Calculate model matrix
	*/

	glm::vec3 position = transform->GetPosition ();
	glm::vec3 scalev = transform->GetScale ();

	glm::mat4 translate = glm::translate (glm::mat4 (1.f), glm::vec3 (position.x, position.y, position.z));
	glm::mat4 scale = glm::scale (glm::mat4 (1.f), glm::vec3 (scalev.x, scalev.y, scalev.z));

	glm::mat4 modelMatrix = translate * scale;

	/*
	 * Iterate over all vertices and create triangle mesh collider
	 * according to object transform
	*/

	for_each_type (ObjectModel*, objModel, *mesh) {
		for (PolygonGroup* polyGroup : *objModel) {
			for (Polygon* polygon : *polyGroup) {
				btVector3 vertices [3];

				for(std::size_t vertexIndex=0;vertexIndex<polygon->VertexCount();vertexIndex++) {
					glm::vec3 vertex = mesh->GetVertex (polygon->GetVertex (vertexIndex));

					vertex = glm::vec3 (modelMatrix * glm::vec4 (vertex, 1));

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