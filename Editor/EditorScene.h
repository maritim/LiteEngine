#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "Core/Singleton/Singleton.h"

#include <glm/vec3.hpp>

#include "SceneGraph/Scene.h"

#include "Renderer/RenderSettings.h"

#include "Systems/Camera/Camera.h"


class EditorScene : public Singleton<EditorScene>
{
	friend Singleton<EditorScene>;

private:
	Camera* _sceneCamera;
	RenderSettings* _renderSettings;
	bool _isActive;

	glm::ivec2 _position;
	glm::ivec2 _size;
	unsigned int _textureID;

	glm::ivec2 _mousePosition;
	bool _isHovered;

	// Change this
	Scene* _lastScene;

public:
	void Init ();

	void Update ();

	void Render ();

	Camera* GetCamera ();

	void SetActive (bool isActive);
	bool IsActive () const;

	glm::ivec2 GetWindowPosition () const;
	glm::ivec2 GetWindowSize () const;
	glm::ivec2 GetWindowMousePosition () const;

	bool IsWindowHovered () const;
private:
	EditorScene ();
	~EditorScene ();
	EditorScene (const EditorScene& other);
	EditorScene& operator=(const EditorScene& other);
};

#endif