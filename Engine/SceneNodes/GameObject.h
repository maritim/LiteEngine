#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "SceneGraph/SceneObject.h"
#include "Systems/Components/ComponentObjectI.h"

#include "Mesh/Model.h"
#include "Core/Resources/Resource.h"

#include "Renderer/RenderObject.h"
#include "Systems/Physics/Rigidbody.h"
#include "Systems/Collision/Collider.h"
#include "Audio/AudioSource.h"

class ENGINE_API GameObject : public SceneObject, public ComponentObjectI
{
protected:
	Resource<Model> _mesh;
	RenderObject* _renderObject;
	Rigidbody* _rigidbody;
	Collider* _collider;
	AudioSource* _audioSource;
public:
	GameObject ();
	~GameObject ();

	void SetActive (bool isActive);

	virtual void AttachMesh (const Resource<Model>& mesh);
	Resource<Model> GetMesh () const;

	Rigidbody* GetRigidbody () const;
	Collider* GetCollider () const;
	AudioSource* GetAudioSource () const;

	void Update ();

	void OnAttachedToScene ();
	void OnDetachedFromScene ();
};

#endif