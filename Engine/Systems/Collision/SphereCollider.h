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

	void Rebuild (Model* mesh, Transform* transform);
protected:
	float GetRadius (Model* mesh, Transform* transform);
};

#endif