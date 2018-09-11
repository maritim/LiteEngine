#include "CylinderCollider.h"

#include <glm/gtc/matrix_transform.hpp>

CylinderCollider::CylinderCollider () :
	_isGenerated (true)
{

}

CylinderCollider::CylinderCollider (float radius, float height) :
	_isGenerated (false)
{
	/*
	 * Bullet cylinder shape is created based on half of the extents
	*/

	btVector3 halfExtents = btVector3 (
		radius,
		height / 2.0f,
		radius
	);

	_collisionShape = new btCylinderShape (halfExtents);
}

void CylinderCollider::Rebuild (Model* mesh, Transform* transform)
{
	if (!_isGenerated) {
		return;
	}

	/*
	 * Compute the bounding cylinder extents of the mesh
	*/

	glm::vec3 extents = GetExtents (mesh, transform);

	/*
	 * Bullet cylinder shape is created based on half of the extents
	*/

	btVector3 halfExtents = btVector3 (
		extents.x / 2.0f,
		extents.y / 2.0f,
		extents.z / 2.0f
	);

	_collisionShape = new btCylinderShape (halfExtents);
}

glm::vec3 CylinderCollider::GetExtents (Model* mesh, Transform* transform)
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