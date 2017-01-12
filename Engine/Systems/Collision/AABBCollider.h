#ifndef AABBCOLLIDER_H
#define AABBCOLLIDER_H

#include "Collider.h"

class AABBCollider : public Collider
{
public:
	~AABBCollider ();

	void virtual Rebuild (Model* mesh, Transform* transform);
};

#endif