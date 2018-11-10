#include "SphereCollider.h"

#include <glm/gtc/matrix_transform.hpp>

SphereCollider::SphereCollider () :
	_isGenerated (true)
{

}

SphereCollider::SphereCollider (float radius) :
	_isGenerated (false)
{
	/*
	 * Bullet sphere shape is created based on radius
	*/

	_collisionShape = new btSphereShape (radius);
}

void SphereCollider::Rebuild (Model* mesh, Transform* transform)
{
	if (!_isGenerated) {
		return;
	}

	/*
	 * Compute the bounding sphere radius of the mesh
	*/

	float radius = GetRadius (mesh, transform);

	/*
	 * Bullet sphere shape is created based on radius
	*/

	_collisionShape = new btSphereShape (radius);
}

float SphereCollider::GetRadius (Model* mesh, Transform* transform)
{
	float radius = 0.0f;

	/*
	 * Calculate model matrix
	*/

	glm::vec3 position = transform->GetPosition ();
	glm::vec3 scalev = transform->GetScale ();

	glm::mat4 translate = glm::translate (glm::mat4 (1.f), glm::vec3 (position.x, position.y, position.z));
	glm::mat4 scale = glm::scale (glm::mat4 (1.f), glm::vec3 (scalev.x, scalev.y, scalev.z));

	glm::mat4 modelMatrix = translate * scale;

	/*
	 * Compute object center
	*/

	glm::vec3 center = position;

	/*
	 * Iterate over all vertices and compute radius
	 * according to object transform
	*/

	for_each_type (ObjectModel*, objModel, *mesh) {
		for (PolygonGroup* polyGroup : *objModel) {
			for (Polygon* polygon : *polyGroup) {
				for(std::size_t vertexIndex=0;vertexIndex<polygon->VertexCount();vertexIndex++) {
					glm::vec3 vertex = mesh->GetVertex (polygon->GetVertex (vertexIndex));

					vertex = glm::vec3 (modelMatrix * glm::vec4 (vertex, 1));

					float vertexDistance = glm::distance (
						glm::vec2 (center.x, center.z),
						glm::vec2 (vertex.x, vertex.z)
					);
					radius = std::max (radius, vertexDistance);
				}
			}
		}
	}

	return radius;
}