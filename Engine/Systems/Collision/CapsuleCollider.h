#ifndef CAPSULECOLLIDER_H
#define CAPSULECOLLIDER_H

#include "BulletCollider.h"

#include <bullet/BulletCollision/CollisionShapes/btCapsuleShape.h>

class ENGINE_API CapsuleCollider : public BulletCollider
{
public:
	CapsuleCollider (const Resource<Model>& model);
	CapsuleCollider (float radius, float height);
protected:
	void Rebuild (const Resource<Model>& model);

	std::pair<float, float> GetSize (const Resource<Model>& model);
};

#endif