#ifndef CAPSULECOLLIDER_H
#define CAPSULECOLLIDER_H

#include "BulletCollider.h"

#include <bullet/BulletCollision/CollisionShapes/btCapsuleShape.h>

class CapsuleCollider : public BulletCollider
{
protected:
	bool _isGenerated;

public:
	CapsuleCollider ();
	CapsuleCollider (float radius, float height);

	void Rebuild (Model* mesh, Transform* transform);
protected:
	std::pair<float, float> GetSize (Model* mesh, Transform* transform);
};

#endif