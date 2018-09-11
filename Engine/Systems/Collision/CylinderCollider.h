#ifndef CYLINDERCOLLIDER_H
#define CYLINDERCOLLIDER_H

#include "BulletCollider.h"

#include <bullet/BulletCollision/CollisionShapes/btCylinderShape.h>

class CylinderCollider : public BulletCollider
{
protected:
	bool _isGenerated;

public:
	CylinderCollider ();
	CylinderCollider (float radius, float height);

	void Rebuild (Model* mesh, Transform* transform);
protected:
	glm::vec3 GetExtents (Model* mesh, Transform* transform);
};

#endif