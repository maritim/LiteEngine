#include "Transform.h"

Transform* Transform::Default ()
{
	static Transform* defaultTransform = new Transform ();

	return defaultTransform;
}

Transform::Transform () :
	_position (0.0f),
	_rotation (),
	_scale (1.0f),
	_parent (NULL),
	_children (),
	_isDirty (false)
{

}

// Break the connections
Transform::Transform(const Transform& other) :
	_position (other._position),
	_rotation (other._rotation),
	_scale (other._scale),
	_parent (NULL),
	_children (),
	_isDirty (false)
{

}

void Transform::SetParent (Transform* parent)
{
	_parent = parent;
	_parent->AttachChild (this);

	_position -= _parent->GetPosition ();

	_isDirty = true;
}

void Transform::DetachParent ()
{
	if (_parent == NULL) {
		return ;
	}

	_parent->DetachChild (this);

	_position += _parent->GetPosition ();

	_parent = NULL;

	_isDirty = true;
}

glm::vec3 Transform::GetPosition () const
{
	glm::vec3 position (_position);

	for (Transform* parent = _parent; parent;parent = parent->_parent) {
		position += parent->_position;
	}

	return position;
}

glm::quat Transform::GetRotation () const
{
	return _rotation;
}

glm::vec3 Transform::GetScale () const
{
	glm::vec3 scale (_scale);

	for (Transform* parent = _parent;parent;parent = parent->_parent) {
		scale *= parent->_scale;
	}

	return scale;
}

void Transform::SetPosition (const glm::vec3& position)
{
	_position = position;

	_isDirty = true;
}

void Transform::SetRotation (const glm::quat& rotation)
{
	_rotation = rotation;

	_isDirty = true;
}

void Transform::SetScale (const glm::vec3& scale)
{
	_scale = scale;

	_isDirty = true;
}

Transform & Transform::operator=(const Transform& other)
{
	_position = other._position;
	_rotation = other._rotation;
	_scale = other._scale;

	_isDirty = true;

	return *this;
}

bool Transform::IsDirty () const
{
	return _isDirty;
}

void Transform::SetIsDirty (bool isDirty)
{
	_isDirty = isDirty;
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