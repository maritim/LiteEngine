#include "Transform.h"

#include <glm/gtx/matrix_decompose.hpp>

Transform* Transform::Default ()
{
	static Transform* defaultTransform = new Transform (nullptr);

	return defaultTransform;
}

Transform::Transform (SceneObject* sceneObject) :
	_sceneObject (sceneObject),
	_localPosition (0.0f),
	_localRotation (glm::identity<glm::quat> ()),
	_localScale (1.0f),
	_parent (nullptr),
	_children (),
	_isDirty (false)
{
	UpdateModelMatrix ();
}

// Break the connections
Transform::Transform(const Transform& other) :
	_localPosition (other._localPosition),
	_localRotation (other._localRotation),
	_localScale (other._localScale),
	_parent (nullptr),
	_children (),
	_isDirty (false)
{
	UpdateModelMatrix ();
}

void Transform::SetParent (Transform* parent)
{
	if (_parent != nullptr) {
		_parent->DetachChild (this);
	}

	_parent = parent;

	if (_parent != nullptr) {
		_parent->AttachChild (this);
	}

	_localPosition = GetLocalPosition (_position);
	_localRotation = GetLocalRotation (_rotation);
	_localScale = GetLocalScale (_scale);

	UpdateChildren ();
}

void Transform::DetachParent ()
{
	if (_parent == nullptr) {
		return ;
	}

	_parent->DetachChild (this);

	_parent = nullptr;

	UpdateChildren ();
}

Transform* Transform::GetParent () const
{
	return _parent;
}

std::size_t Transform::GetChildrenCount () const
{
	return _children.size ();
}

SceneObject* Transform::GetSceneObject () const
{
	return _sceneObject;
}

glm::vec3 Transform::GetPosition () const
{
	return _position;
}

glm::vec3 Transform::GetLocalPosition () const
{
	return _localPosition;
}

glm::quat Transform::GetRotation () const
{
	return _rotation;
}

glm::quat Transform::GetLocalRotation () const
{
	return _localRotation;
}

glm::vec3 Transform::GetScale () const
{
	return _scale;
}

glm::vec3 Transform::GetLocalScale () const
{
	return _localScale;
}

void Transform::SetPosition (const glm::vec3& position)
{
	_localPosition = GetLocalPosition (position);

	UpdateModelMatrix ();
}

void Transform::SetRotation (const glm::quat& rotation)
{
	_localRotation = GetLocalRotation (rotation);

	UpdateModelMatrix ();
}

void Transform::SetScale (const glm::vec3& scale)
{
	_localScale = GetLocalScale (scale);

	UpdateModelMatrix ();
}

void Transform::SetLocalPosition (const glm::vec3& localPosition)
{
	_localPosition = localPosition;

	UpdateModelMatrix ();
}

void Transform::SetLocalRotation (const glm::quat& localRotation)
{
	_localRotation = localRotation;

	UpdateModelMatrix ();
}

void Transform::SetLocalScale (const glm::vec3& localScale)
{
	_localScale = localScale;

	UpdateModelMatrix ();
}

Transform & Transform::operator=(const Transform& other)
{
	_localPosition = other._localPosition;
	_localRotation = other._localRotation;
	_localScale = other._localScale;

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

const glm::mat4& Transform::GetModelMatrix () const
{
	return _modelMatrix;
}

void Transform::AttachChild (Transform* transform)
{
	_children.insert (transform);
}

void Transform::DetachChild (Transform* transform)
{
	_children.erase (transform);

	transform->UpdateChildren ();
}

void Transform::UpdateModelMatrix ()
{
	_localModelMatrix = glm::scale (glm::mat4 (1.0f), _localScale);
	_localModelMatrix = glm::mat4_cast(_localRotation) * _localModelMatrix;
	_localModelMatrix = glm::translate (glm::mat4 (1.0f), _localPosition) * _localModelMatrix;

	UpdateChildren ();
}

void Transform::UpdateChildren ()
{
	if (_parent == nullptr) {
		_modelMatrix = _localModelMatrix;

		_position = _localPosition;
		_rotation = _localRotation;
		_scale = _localScale;
	} else {
		_modelMatrix = _parent->_modelMatrix * _localModelMatrix;

		glm::vec3 dummy1;
		glm::vec4 dummy2;

		glm::decompose (_modelMatrix, _scale, _rotation, _position, dummy1, dummy2);
	}

	for (auto child : _children) {
		child->UpdateChildren ();
	}

	_isDirty = true;
}

glm::vec3 Transform::GetLocalPosition (const glm::vec3& newPosition)
{
	if (_parent == nullptr) {
		return newPosition;
	}

	glm::mat4 translation = glm::inverse (_parent->_modelMatrix) *
		glm::translate (glm::mat4 (1), newPosition) *
		glm::inverse (glm::mat4_cast(_localRotation)) *
		glm::inverse (glm::scale (glm::mat4 (1.0f), _localScale));

	return glm::vec3 (translation [3][0], translation [3][1], translation [3][2]);
}

glm::quat Transform::GetLocalRotation (const glm::quat& newRotation)
{
	if (_parent == nullptr) {
		return newRotation;
	}

	glm::mat4 rotationMatrix = glm::inverse (_parent->_modelMatrix) *
		glm::inverse (glm::translate (glm::mat4 (1), _localPosition)) *
		glm::mat4_cast (newRotation) *
		glm::inverse (glm::scale (glm::mat4 (1), _localScale));

	glm::vec3 dummy1;
	glm::vec4 dummy2;

	glm::quat rotation;

	glm::decompose (rotationMatrix, dummy1, rotation, dummy1, dummy1, dummy2);

	return rotation;
}

glm::vec3 Transform::GetLocalScale (const glm::vec3& newScale)
{
	if (_parent == nullptr) {
		return newScale;
	}

	glm::mat4 scaleMatrix = glm::inverse (_parent->_modelMatrix) *
		glm::inverse (glm::translate (glm::mat4 (1), _localPosition)) *
		glm::inverse (glm::mat4_cast(_localRotation)) *
		glm::scale (glm::mat4 (1), newScale);

	glm::vec3 dummy1;
	glm::vec4 dummy2;
	glm::quat dummy3;

	glm::vec3 scale;

	glm::decompose (scaleMatrix, scale, dummy3, dummy1, dummy1, dummy2);

	return scale;
}
