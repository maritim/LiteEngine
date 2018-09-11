#include "BoxCollider.h"

#include <glm/gtc/matrix_transform.hpp>

BoxCollider::BoxCollider () :
	_isGenerated (true)
{

}

BoxCollider::BoxCollider (const glm::vec3& extents) :
	_isGenerated (false)
{
	/*
	 * Bullet box shape is created based on half of the extents
	*/

	btVector3 halfExtents = btVector3 (
		extents.x / 2.0f,
		extents.y / 2.0f,
		extents.z / 2.0f
	);

	_collisionShape = new btBoxShape (halfExtents);
}

void BoxCollider::Rebuild (Model* mesh, Transform* transform)
{
	if (!_isGenerated) {
		return;
	}

	/*
	 * Compute mesh extents
	*/

	glm::vec3 extents = GetExtents (mesh, transform);

	/*
	 * Bullet box shape is created based on half of the extents
	*/

	btVector3 halfExtents = btVector3 (
		extents.x / 2.0f,
		extents.y / 2.0f,
		extents.z / 2.0f
	);

	_collisionShape = new btBoxShape (halfExtents);
}

glm::vec3 BoxCollider::GetExtents (Model* mesh, Transform* transform)
{
	/*
	 * Get mesh bounding box
	*/

	BoundingBox* boundingBox = mesh->GetBoundingBox ();

	/*
	 * Calculate model matrix
	*/

	glm::vec3 position = transform->GetPosition ();
	glm::vec3 scalev = transform->GetScale ();

	glm::mat4 translate = glm::translate (glm::mat4 (1.f), glm::vec3 (position.x, position.y, position.z));
	glm::mat4 scale = glm::scale (glm::mat4 (1.f), glm::vec3 (scalev.x, scalev.y, scalev.z));

	glm::mat4 modelMatrix = translate * scale;

	/*
	 * Compute mesh extents according to its world position
	*/

	glm::vec3 minVertex = glm::vec3 (boundingBox->xmin, boundingBox->ymin, boundingBox->zmin);
	glm::vec3 maxVertex = glm::vec3 (boundingBox->xmax, boundingBox->ymax, boundingBox->zmax);

	minVertex = glm::vec3 (modelMatrix * glm::vec4 (minVertex, 1.0f));
	maxVertex = glm::vec3 (modelMatrix * glm::vec4 (maxVertex, 1.0f));

	glm::vec3 extents = maxVertex - minVertex;

	return extents;
}