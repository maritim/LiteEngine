#include "CameraManager.h"

CameraManager::CameraManager () :
	_active (nullptr)
{

}

CameraManager::~CameraManager ()
{

}

SPECIALIZE_SINGLETON(CameraManager)

void CameraManager::SetActive (Camera* camera)
{
	_active = camera;
}

Camera* CameraManager::GetActive ()
{
	return _active;
}
