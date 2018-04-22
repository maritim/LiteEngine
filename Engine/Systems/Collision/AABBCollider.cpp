#include "AABBCollider.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Core/Intersections/AABBVolume.h"

AABBCollider::~AABBCollider ()
{

}

#include "Debug/Logger/Logger.h"
#include <glm/gtx/string_cast.hpp>

void AABBCollider::Rebuild (Model* mesh, Transform* transform)
{
	BoundingBox* boundingBox = mesh->GetBoundingBox ();

	AABBVolume::AABBVolumeInformation* aabbPrimitive = 
		new AABBVolume::AABBVolumeInformation ();

	aabbPrimitive->minVertex.x = boundingBox->xmin;
	aabbPrimitive->maxVertex.x = boundingBox->xmax;
	aabbPrimitive->minVertex.y = boundingBox->ymin;
	aabbPrimitive->maxVertex.y = boundingBox->ymax;
	aabbPrimitive->minVertex.z = boundingBox->zmin;
	aabbPrimitive->maxVertex.z = boundingBox->zmax;

	/*
	 * Calculate model matrix.
	*/

	glm::vec3 position = transform->GetPosition ();
	glm::vec3 scalev = transform->GetScale ();

	glm::mat4 translate = glm::translate (glm::mat4 (1.f), glm::vec3 (position.x, position.y, position.z));
	glm::mat4 scale = glm::scale (glm::mat4 (1.f), glm::vec3 (scalev.x, scalev.y, scalev.z));

	glm::mat4 modelMatrix = translate * scale;

	/*
	 * Apply transform to bounding box
	*/

	aabbPrimitive->minVertex = glm::vec3 (modelMatrix * glm::vec4 (aabbPrimitive->minVertex, 1));
	aabbPrimitive->maxVertex = glm::vec3 (modelMatrix * glm::vec4 (aabbPrimitive->maxVertex, 1));

	/*
	 * Create new AABB Volume
	*/

	AABBVolume* aabb = new AABBVolume (aabbPrimitive);

	/*
	 * Replace Current Volume
	*/

	// DEBUG_LOG (glm::to_string (aabbPrimitive->minVertex));
	// DEBUG_LOG (glm::to_string (aabbPrimitive->maxVertex));

	DestroyCurrentPrimitive ();
	_primitive = aabb;
}