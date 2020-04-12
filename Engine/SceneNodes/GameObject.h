#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "SceneGraph/SceneObject.h"

#include "Systems/Collision/Collider.h"

class ENGINE_API GameObject : public SceneObject
{
protected:
	Collider* _collider;
public:
	GameObject ();
	~GameObject ();

	void SetActive (bool isActive);

	Collider* GetCollider () const;

	void Update ();

	void OnAttachedToScene ();
	void OnDetachedFromScene ();
};

#endif