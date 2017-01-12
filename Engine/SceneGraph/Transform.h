#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>

#include "Core/Math/glm/vec3.hpp"
#include "Core/Math/glm/gtc/quaternion.hpp"

class Transform
{
private:
	glm::vec3 _position;
	glm::quat _rotation;
	glm::vec3 _scale;

	Transform* _parent;
	std::vector<Transform*> _children;

	bool _isDirty;

public:
	Transform ();
	Transform (const Transform& other);

	static Transform* Default ();

	void SetParent (Transform* parent);
	void DetachParent ();

	glm::vec3 GetPosition () const;
	glm::quat GetRotation () const;
	glm::vec3 GetScale () const;

	glm::vec3 GetLocalPosition () const;
	glm::quat GetLocalRotation () const;
	glm::vec3 GetLocalScale () const;

	void SetPosition (const glm::vec3& position);
	void SetRotation (const glm::quat& rotation);
	void SetScale (const glm::vec3& scale);

	bool IsDirty () const;
	void SetIsDirty (bool isDirty);

	Transform & operator=(const Transform& other);
private:
	void AttachChild (Transform* transform);
	void DetachChild (Transform* transform);
};

#endif