#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>

#include "Core/Math/Vector3.h"

class Transform
{
private:
	Vector3 _position;
	Vector3 _rotation;
	Vector3 _scale;

	Transform* _parent;
	std::vector<Transform*> _children;

public:
	Transform ();
	Transform (const Transform& other);

	static Transform* Default ();

	void SetParent (Transform* parent);
	void DetachParent ();

	Vector3 GetPosition () const;
	Vector3 GetRotation () const;
	Vector3 GetScale () const;

	Vector3 GetLocalPosition () const;
	Vector3 GetLocalRotation () const;
	Vector3 GetLocalScale () const;

	void SetPosition (const Vector3& position);
	void SetRotation (const Vector3& rotation);
	void SetScale (const Vector3& scale);

	Transform & operator=(const Transform& other);
private:
	void AttachChild (Transform* transform);
	void DetachChild (Transform* transform);
};

#endif