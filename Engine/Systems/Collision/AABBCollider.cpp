#include "AABBCollider.h"

#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include "Core/Intersections/AABBVolume.h"

AABBCollider::~AABBCollider ()
{

}

void AABBCollider::Rebuild (Model* mesh, Transform* transform)
{
	BoundingBox* boundingBox = mesh->GetBoundingBox ();

	AABBVolume::AABBVolumeInformation* aabbPrimitive = 
		new AABBVolume::AABBVolumeInformation ();

	aabbPrimitive->minVertex = glm::vec3 (std::numeric_limits<float>::infinity ());
	aabbPrimitive->maxVertex = glm::vec3 (-std::numeric_limits<float>::infinity ());

	/*
	 * Calculate model matrix.
	*/

	glm::vec3 position = transform->GetPosition ();
	glm::vec3 scalev = transform->GetScale ();
	glm::quat rotationq = transform->GetRotation ();

	glm::mat4 translate = glm::translate (glm::mat4 (1.f), glm::vec3 (position.x, position.y, position.z));
	glm::mat4 scale = glm::scale (glm::mat4 (1.f), glm::vec3 (scalev.x, scalev.y, scalev.z));
	glm::mat4 rotation = glm::mat4_cast (rotationq);

	glm::mat4 modelMatrix = translate * scale * rotation;

	/*
	 * Calculate AABB from model bounding box
	*/

	float vertices [2][3] = {
		{ boundingBox->xmin, boundingBox->ymin, boundingBox->zmin },
		{ boundingBox->xmax, boundingBox->ymax, boundingBox->zmax }
	};

	for (std::size_t i = 0; i < 2; i++) {
		for (std::size_t j = 0; j < 2; j++) {
			for (std::size_t k = 0; k < 2; k++) {

				glm::vec3 vertex = glm::vec3 (vertices [i][0], vertices [j][1], vertices [k][2]);

				vertex = glm::vec3 (modelMatrix * glm::vec4 (vertex, 1));

				aabbPrimitive->minVertex.x = std::min (aabbPrimitive->minVertex.x, vertex.x);
				aabbPrimitive->minVertex.y = std::min (aabbPrimitive->minVertex.y, vertex.y);
				aabbPrimitive->minVertex.z = std::min (aabbPrimitive->minVertex.z, vertex.z);
				aabbPrimitive->maxVertex.x = std::max (aabbPrimitive->maxVertex.x, vertex.x);
				aabbPrimitive->maxVertex.y = std::max (aabbPrimitive->maxVertex.y, vertex.y);
				aabbPrimitive->maxVertex.z = std::max (aabbPrimitive->maxVertex.z, vertex.z);
			}
		}
	}

	/*
	 * Create new AABB Volume
	*/

	AABBVolume* aabb = new AABBVolume (aabbPrimitive);

	/*
	 * Replace Current Volume
	*/

	DestroyCurrentPrimitive ();
	_primitive = aabb;
}