#ifndef MESHCOLLIDER_H
#define MESHCOLLIDER_H

#include "BulletCollider.h"

#include <bullet/BulletCollision/CollisionShapes/btTriangleMesh.h>

class ENGINE_API MeshCollider : public BulletCollider
{
protected:
	btTriangleMesh* _triangleMesh;

public:
	MeshCollider (const Resource<Model>& model);
	~MeshCollider ();
protected:
	void Rebuild (const Resource<Model>& model);

	btTriangleMesh* GetTriangleMesh (const Resource<Model>& model);

	void DestroyTriangleMesh ();
};

#endif