#ifndef SPHERECOLLIDER_H
#define SPHERECOLLIDER_H

#include "BulletCollider.h"

#include <bullet/BulletCollision/CollisionShapes/btSphereShape.h>

class SphereCollider : public BulletCollider
{
protected:
	bool _isGenerated;

public:
	SphereCollider ();
	SphereCollider (float radius);
protected:
	void Rebuild ();

	float GetRadius ();
};

#endif