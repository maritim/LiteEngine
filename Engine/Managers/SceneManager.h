#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <string>

#include "SceneGraph/Scene.h"

class ENGINE_API SceneManager : public Singleton<SceneManager>
{
	friend Singleton<SceneManager>;

	DECLARE_SINGLETON(SceneManager)

private:
	Scene* _current;

	bool _needToLoad;
	std::string _nextSceneName;

public:
	void Update ();

	Scene* Current ();
	void Load (const std::string&);
	void Clear();

private:
	SceneManager ();
	~SceneManager ();
	SceneManager (const SceneManager&);
	SceneManager& operator=(const SceneManager&);

	void LoadNextScene (const std::string&);
};

#endif