#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Core/Interfaces/Object.h"

#include <string>

#include "Transform.h"
#include "Renderer/Renderer.h"
#include "Systems/Physics/Rigidbody.h"
#include "Systems/Collision/Collider.h"

class SceneObject : public Object
{
protected:
	std::string _name;
	std::size_t _instanceID;
	Transform* _transform;
	Renderer* _renderer;
	Rigidbody* _rigidbody;
	Collider* _collider;

public:
	SceneObject ();
	
	std::string GetName () const;
	std::size_t GetInstanceID () const;

	void SetName (const std::string& name);
	void SetInstanceID (std::size_t instanceID);

	Transform* GetTransform () const;
	Renderer* GetRenderer () const;
	Rigidbody* GetRigidbody () const;
	Collider* GetCollider () const;

	virtual void Update () = 0;

	virtual void OnAttachedToScene ();
	virtual void OnDetachedFromScene ();

	virtual ~SceneObject ();
};

#endif