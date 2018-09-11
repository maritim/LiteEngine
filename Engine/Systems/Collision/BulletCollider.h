#ifndef BULLETCOLLIDER_H
#define BULLETCOLLIDER_H

#include "Core/Interfaces/Object.h"

#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <glm/vec3.hpp>

#include "Mesh/Model.h"
#include "SceneGraph/Transform.h"

class BulletCollider : public Object
{
protected:
	btCollisionShape* _collisionShape;
	glm::vec3 _offset;

public:
	BulletCollider ();
	virtual ~BulletCollider ();

	virtual void Rebuild (Model* mesh, Transform* transform) = 0;

	void SetOffset (const glm::vec3& offset);

	btCollisionShape* GetCollisionShape () const;
	glm::vec3 GetOffset () const;
protected:
	void DestroyCollisionShape ();
};

#endif