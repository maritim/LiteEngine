#ifndef AABBCOLLIDER_H
#define AABBCOLLIDER_H

#include "Collider.h"

class ENGINE_API AABBCollider : public Collider
{
public:
	~AABBCollider ();

	void virtual Rebuild (const Resource<Model>& mesh, Transform* transform);
};

#endif