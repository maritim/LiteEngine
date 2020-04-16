#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Core/Interfaces/Object.h"

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <set>

#include "SceneObject.h"

#include "Core/Iteration/MultipleContainer.h"

class SceneObject;

class ENGINE_API Transform : public Object
{
private:
	SceneObject* _sceneObject;

	glm::vec3 _position;
	glm::quat _rotation;
	glm::vec3 _scale;

	glm::vec3 _localPosition;
	glm::quat _localRotation;
	glm::vec3 _localScale;

	Transform* _parent;
	std::set<Transform*> _children;

	bool _isDirty;

	glm::mat4 _localModelMatrix;
	glm::mat4 _modelMatrix;

public:
	Transform (SceneObject* sceneObject);
	Transform (const Transform& other);

	static Transform* Default ();

	void SetParent (Transform* parent);
	void DetachParent ();

	Transform* GetParent () const;

	std::size_t GetChildrenCount () const;

	SceneObject* GetSceneObject () const;

	const glm::vec3& GetPosition () const;
	const glm::quat& GetRotation () const;
	const glm::vec3& GetScale () const;

	const glm::vec3& GetLocalPosition () const;
	const glm::quat& GetLocalRotation () const;
	const glm::vec3& GetLocalScale () const;

	void SetPosition (const glm::vec3& position);
	void SetRotation (const glm::quat& rotation);
	void SetScale (const glm::vec3& scale);

	void SetLocalPosition (const glm::vec3& localPosition);
	void SetLocalRotation (const glm::quat& localRotation);
	void SetLocalScale (const glm::vec3& localScale);

	bool IsDirty () const;
	void SetIsDirty (bool isDirty);

	Transform & operator=(const Transform& other);

	const glm::mat4& GetModelMatrix () const;

	MULTIPLE_CONTAINER_TEMPLATE (set)
private:
	void AttachChild (Transform* transform);
	void DetachChild (Transform* transform);

	void UpdateModelMatrix ();
	void UpdateChildren ();

	glm::vec3 GetLocalPosition (const glm::vec3& newPosition);
	glm::quat GetLocalRotation (const glm::quat& newRotation);
	glm::vec3 GetLocalScale (const glm::vec3& newScale);
};

MULTIPLE_CONTAINER_SPECIALIZATION (set, Transform*, Transform, _children);

#endif