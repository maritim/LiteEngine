#ifndef SPHERECOLLIDER_H
#define SPHERECOLLIDER_H

#include "BulletCollider.h"

#include <bullet/BulletCollision/CollisionShapes/btSphereShape.h>

class ENGINE_API SphereCollider : public BulletCollider
{
public:
	SphereCollider (const Resource<Model>& model);
	SphereCollider (float radius);
protected:
	void Rebuild (const Resource<Model>& model);

	float GetRadius (const Resource<Model>& model);
};

#endif