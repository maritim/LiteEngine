#include "CameraComponent.h"

#include "SceneGraph/SceneObject.h"

#include "Managers/CameraManager.h"

CameraComponent::CameraComponent () :
	_camera (nullptr)
{

}

CameraComponent::~CameraComponent ()
{
	delete _camera;
}

void CameraComponent::Update ()
{
	if (_camera == nullptr) {
		return;
	}

	if (_parent->GetTransform ()->IsDirty ()) {
		_camera->SetPosition (_parent->GetTransform ()->GetPosition ());
		_camera->SetRotation (_parent->GetTransform ()->GetRotation ());
	}

	_camera->Update ();
}

void CameraComponent::SetActive (bool isActive)
{
	if (isActive == true) {
		CameraManager::Instance ()->SetActive (_camera);
	}

	if (isActive == false) {
		CameraManager::Instance ()->SetActive (nullptr);
	}
}

void CameraComponent::OnAttachedToScene ()
{
	CameraManager::Instance ()->SetActive (_camera);
}

void CameraComponent::OnDetachedFromScene ()
{
	CameraManager::Instance ()->SetActive (nullptr);
}

float CameraComponent::GetAspect () const
{
	return _aspect;
}

float CameraComponent::GetZNear () const
{
	return _zNear;
}

float CameraComponent::GetZFar () const
{
	return _zFar;
}

bool CameraComponent::GetConstrainAspect () const
{
	return _constrainAspect;
}

void CameraComponent::SetAspect (float aspect)
{
	_aspect = aspect;

	_camera->SetAspect (_aspect);
}

void CameraComponent::SetZNear (float zNear)
{
	_zNear = zNear;

	_camera->SetZNear (_zNear);
}

void CameraComponent::SetZFar (float zFar)
{
	_zFar = zFar;

	_camera->SetZFar (_zFar);
}

void CameraComponent::SetConstrainAspect (bool constrainAspect)
{
	_constrainAspect = constrainAspect;

	_camera->SetConstrainAspect (_constrainAspect);
}
