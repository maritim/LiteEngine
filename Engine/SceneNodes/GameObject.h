#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "SceneGraph/SceneObject.h"

#include "Mesh/Model.h"
#include "Core/Resources/Resource.h"

#include "Systems/Physics/Rigidbody.h"
#include "Systems/Collision/Collider.h"
#include "Audio/AudioSource.h"

class ENGINE_API GameObject : public SceneObject
{
protected:
	Rigidbody* _rigidbody;
	Collider* _collider;
	AudioSource* _audioSource;
public:
	GameObject ();
	~GameObject ();

	void SetActive (bool isActive);

	Rigidbody* GetRigidbody () const;
	Collider* GetCollider () const;
	AudioSource* GetAudioSource () const;

	void Update ();

	void OnAttachedToScene ();
	void OnDetachedFromScene ();
};

#endif