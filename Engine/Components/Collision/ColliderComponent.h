#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "Systems/Components/Component.h"

#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <bullet/BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <glm/vec3.hpp>

#include "Core/Resources/Resource.h"
#include "Renderer/Render/Mesh/Model.h"

class ENGINE_API ColliderComponent : public Component
{
	DECLARE_COMPONENT(ColliderComponent)

protected:
	ATTRIBUTE(EditAnywhere, Meta)
	glm::vec3 _offset;

	btCollisionShape* _collisionShape;
	btCollisionObject* _collisionObject;

	bool _isSelected;
	bool _isSelectedLastFrame;

public:
	ColliderComponent ();
	~ColliderComponent ();

	void Awake ();

	void Update ();

	void SetActive (bool isActive);

	void OnGizmo ();

	void OnAttachedToScene ();
	void OnDetachedFromScene ();

	virtual void SetModel (const Resource<Model>& model);
	virtual void SetOffset (const glm::vec3& offset);

	const glm::vec3& GetOffset () const;

	btCollisionShape* GetCollisionShape () const;
};

#endif