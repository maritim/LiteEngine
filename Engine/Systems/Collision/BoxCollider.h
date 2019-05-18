#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "BulletCollider.h"

#include <bullet/BulletCollision/CollisionShapes/btBoxShape.h>

class ENGINE_API BoxCollider : public BulletCollider
{
protected:
	bool _isGenerated;

public:
	BoxCollider ();
	BoxCollider (const glm::vec3& extents);
protected:
	void Rebuild ();

	glm::vec3 GetExtents ();
};

#endif