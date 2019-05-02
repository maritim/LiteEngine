#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <string>

#include "Systems/Camera/Camera.h"

class CameraManager : public Singleton<CameraManager>
{
	friend Singleton<CameraManager>;

private:
	Camera* _active;

public:
	void Update ();

	void SetActive (Camera* camera);
	Camera* GetActive ();

private:
	CameraManager ();
	~CameraManager ();
	CameraManager (const CameraManager&);
	CameraManager& operator=(const CameraManager&);
};

#endif