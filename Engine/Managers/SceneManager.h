#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <string>

#include "SceneGraph/Scene.h"

class SceneManager : public Singleton<SceneManager>
{
	friend Singleton<SceneManager>;

private:
	Scene* _current;

public:
	Scene* Current ();
	Scene* Load (const std::string&);
	void Clear();

private:
	SceneManager ();
	~SceneManager ();
	SceneManager (const SceneManager&);
	SceneManager& operator=(const SceneManager&);
};

#endif