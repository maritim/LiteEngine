#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "SceneGraph/Scene.h"

class PhysicsSystem
{
protected:
	Scene* _currentScene;

public:
	static PhysicsSystem& Instance ();

	void Init (Scene* scene);

	void UpdateScene ();
private:
	PhysicsSystem ();
};

#endif