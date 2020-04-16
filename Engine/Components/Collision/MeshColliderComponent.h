#ifndef MESHCOLLIDERCOMPONENT_H
#define MESHCOLLIDERCOMPONENT_H

#include "ColliderComponent.h"

#include <bullet/BulletCollision/CollisionShapes/btTriangleMesh.h>

class ENGINE_API MeshColliderComponent : public ColliderComponent
{
	DECLARE_COMPONENT(MeshColliderComponent)

protected:
	ATTRIBUTE(EditAnywhere, Meta=(Model))
	Resource<Model> _model;

	btTriangleMesh* _triangleMesh;

public:
	MeshColliderComponent ();
	~MeshColliderComponent ();

	void Awake ();

	void SetModel (const Resource<Model>& model);
protected:
	btTriangleMesh* GetTriangleMesh (const Resource<Model>& model);
};

#endif