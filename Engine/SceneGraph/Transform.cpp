#include "Transform.h"

Transform* Transform::Default ()
{
	static Transform* defaultTransform = new Transform ();

	return defaultTransform;
}

Transform::Transform () :
	_position (Vector3::Zero),
	_rotation (Vector3::Zero),
	_scale (Vector3::One),
	_parent (NULL),
	_children ()
{

}

// Break the connections
Transform::Transform(const Transform& other) :
	_position (other._position),
	_rotation (other._rotation),
	_scale (other._scale),
	_parent (NULL),
	_children ()
{

}

void Transform::SetParent (Transform* parent)
{
	_parent = parent;
	_parent->AttachChild (this);

	_position -= _parent->GetPosition ();
}

void Transform::DetachParent ()
{
	if (_parent == NULL) {
		return ;
	}

	_parent->DetachChild (this);

	_position += _parent->GetPosition ();

	_parent = NULL;
}

Vector3 Transform::GetPosition () const
{
	Vector3 position (_position);

	for (Transform* parent = _parent; parent;parent = parent->_parent) {
		position += parent->_position;
	}

	return position;
}

// TODO: Implement with Quaternions
Vector3 Transform::GetRotation () const
{
	return _rotation;
}

Vector3 Transform::GetScale () const
{
	Vector3 scale (_scale);

	for (Transform* parent = _parent;parent;parent = parent->_parent) {
		scale *= parent->_scale;
	}

	return scale;
}

void Transform::SetPosition (const Vector3& position)
{
	_position = position;
}

void Transform::SetRotation (const Vector3& rotation)
{
	_rotation = rotation;
}

void Transform::SetScale (const Vector3& scale)
{
	_scale = scale;
}

Transform & Transform::operator=(const Transform& other)
{
	_position = other._position;
	_rotation = other._rotation;
	_scale = other._scale;

	return *this;
}

void Transform::AttachChild (Transform* transform)
{
	_children.push_back (this);
}

void Transform::DetachChild (Transform* transform)
{
	for (std::size_t i=0;i<_children.size ();i++) {
		if (_children [i] == this) {
			_children [i] = _children.back ();
			_children.pop_back ();

			break;
		}
	}
}