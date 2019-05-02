#ifndef MESHCOLLIDER_H
#define MESHCOLLIDER_H

#include "BulletCollider.h"

#include <bullet/BulletCollision/CollisionShapes/btTriangleMesh.h>

class MeshCollider : public BulletCollider
{
protected:
	btTriangleMesh* _triangleMesh;

public:
	~MeshCollider ();
protected:
	void Rebuild ();

	btTriangleMesh* GetTriangleMesh ();

	void DestroyTriangleMesh ();
};

#endif