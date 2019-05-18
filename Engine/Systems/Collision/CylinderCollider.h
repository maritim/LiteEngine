#ifndef CYLINDERCOLLIDER_H
#define CYLINDERCOLLIDER_H

#include "BulletCollider.h"

#include <bullet/BulletCollision/CollisionShapes/btCylinderShape.h>

class ENGINE_API CylinderCollider : public BulletCollider
{
protected:
	bool _isGenerated;

public:
	CylinderCollider ();
	CylinderCollider (float radius, float height);
protected:
	void Rebuild ();

	glm::vec3 GetExtents ();
};

#endif