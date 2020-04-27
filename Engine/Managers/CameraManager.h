#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <string>

#include "Systems/Camera/Camera.h"

class ENGINE_API CameraManager : public Singleton<CameraManager>
{
	friend Singleton<CameraManager>;

	DECLARE_SINGLETON(CameraManager)

private:
	Camera* _active;

public:
	void SetActive (Camera* camera);
	Camera* GetActive ();

private:
	CameraManager ();
	~CameraManager ();
	CameraManager (const CameraManager&);
	CameraManager& operator=(const CameraManager&);
};

#endif