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

	void Rebuild (Model* mesh, Transform* transform);
protected:
	btTriangleMesh* GetTriangleMesh (Model* mesh, Transform* transform);

	void DestroyTriangleMesh ();
};

#endif