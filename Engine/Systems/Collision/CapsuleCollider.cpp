#include "CapsuleCollider.h"

#include <glm/gtc/matrix_transform.hpp>

CapsuleCollider::CapsuleCollider () :
	_isGenerated (true)
{

}

CapsuleCollider::CapsuleCollider (float radius, float height) :
	_isGenerated (false)
{
	/*
	 * Bullet capsule shape is created based on radius and height
	*/

	_collisionShape = new btCapsuleShape (radius, height);
}

void CapsuleCollider::Rebuild (Model* mesh, Transform* transform)
{
	if (!_isGenerated) {
		return;
	}

	/*
	 * Compute the bounding capsule extents of the mesh
	*/

	std::pair<float, float> size = GetSize (mesh, transform);

	/*
	 * Bullet capsule shape is created based on radius and height
	*/

	_collisionShape = new btCapsuleShape (size.first, size.second);
}

std::pair<float, float> CapsuleCollider::GetSize (Model* mesh, Transform* transform)
{
	float radius = 0.0f;
	float height = 0.0f;

	float minY = std::numeric_limits<float>::max ();
	float maxY = -std::numeric_limits<float>::max ();

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

	glm::vec3 center = glm::vec3 (modelMatrix * glm::vec4 (position, 1));

	/*
	 * Iterate over all vertices and compute radius and Y limits
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

					minY = std::min (minY, vertex.y);
					maxY = std::max (maxY, vertex.y);
				}
			}
		}
	}

	/*
	 * Compute capsule height according to its actual size
	 *
	 * actualHeight = height + 2 * radius
	*/

	height = maxY - minY - radius * 2.0f;

	return std::pair<float, float> (radius, height);
}