#ifndef EDITORGAME_H
#define EDITORGAME_H

#include "Core/Singleton/Singleton.h"

#include <glm/vec3.hpp>

#include "SceneGraph/Scene.h"

#include "Renderer/RenderSettings.h"

#include "Systems/Camera/Camera.h"


class EditorGame : public Singleton<EditorGame>
{
	friend Singleton<EditorGame>;

private:
	Camera* _sceneCamera;
	RenderSettings* _renderSettings;
	bool _isActive;

	glm::ivec2 _position;
	glm::ivec2 _size;
	unsigned int _textureID;

	glm::ivec2 _mousePosition;
	bool _isHovered;

	float _targetFrameRate;
	float _elapsedFrameTime;

	// Change this
	Scene* _lastScene;

public:
	void Init();

	void Update();

	void Render();

	Camera* GetCamera();

	void SetActive(bool isActive);
	bool IsActive() const;

	glm::ivec2 GetWindowPosition() const;
	glm::ivec2 GetWindowSize() const;
	glm::ivec2 GetWindowMousePosition() const;

	bool IsWindowHovered() const;
private:
	EditorGame();
	~EditorGame();
	EditorGame(const EditorGame& other);
	EditorGame& operator=(const EditorGame& other);

	void ShowSceneMenu();
	void ShowNoCameraMenu();
};
#endif