#include "CameraManager.h"

//TODO: Change this
#include "Cameras/PerspectiveCamera.h"

CameraManager::CameraManager ()
{
	_active = new PerspectiveCamera ();

	auto camera = (PerspectiveCamera*) _active;

	camera->SetFieldOfView (45.0f);
	camera->SetConstraintAspect (true);
}

CameraManager::~CameraManager ()
{

}

SPECIALIZE_SINGLETON(CameraManager)

void CameraManager::Update ()
{
	_active->Update ();
}

void CameraManager::SetActive (Camera* camera)
{
	_active = camera;
}

Camera* CameraManager::GetActive ()
{
	return _active;
}