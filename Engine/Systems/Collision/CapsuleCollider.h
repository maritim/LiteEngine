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
protected:
	void Rebuild ();

	std::pair<float, float> GetSize ();
};

#endif