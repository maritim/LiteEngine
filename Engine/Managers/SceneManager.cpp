#include "SceneManager.h"

#include "Resources/SceneLoader.h"

#include "Core/Console/Console.h"

#include "Systems/Components/ComponentManager.h"

#define SCENE_LOADING_ERROR_CODE 10

SceneManager::SceneManager () :
	_current (nullptr),
	_needToLoad (false),
	_nextSceneName ()
{

}

SceneManager::~SceneManager ()
{
	Clear ();
}

SPECIALIZE_SINGLETON(SceneManager)

Scene* SceneManager::Current ()
{
	return _current;
}

/*
 * TODO: Improve this to manage more cleaning stuff
*/

void SceneManager::Update ()
{
	if (_needToLoad == true) {
		LoadNextScene (_nextSceneName);

		_needToLoad = false;
	}
}

void SceneManager::Load (const std::string& sceneName)
{
	_needToLoad = true;
	_nextSceneName = sceneName;
}

void SceneManager::Clear ()
{
	if (_current == nullptr) {
		return ;
	}

	delete _current;
	_current = nullptr;
}

void SceneManager::LoadNextScene (const std::string& sceneName)
{
	if (_current != nullptr) {
		delete _current;

		ComponentManager::Instance ()->Clear ();
	}

	_current = SceneLoader::Instance ().Load (sceneName);

	if (_current == nullptr) {
		Console::LogError ("An error occured while tryng to load " + sceneName);
		std::exit(SCENE_LOADING_ERROR_CODE);
	}
}