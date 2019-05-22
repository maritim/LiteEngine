#include "EditorScene.h"

#include "Systems/GUI/ImGui/imgui.h"

#include "Renderer/RenderManager.h"

#include "RenderPasses/FrameBuffer2DVolume.h"

#include "Managers/SceneManager.h"
#include "Managers/RenderSettingsManager.h"

#include "Systems/Input/Input.h"

#include "Resources/Resources.h"

#include "Cameras/PerspectiveCamera.h"

EditorScene::EditorScene () :
	_sceneCamera (new PerspectiveCamera ()),
	_renderSettings (nullptr),
	_position (0),
	_size (0),
	_textureID (0),
	_mousePosition (0),
	_isHovered (false)
{

}

EditorScene::~EditorScene ()
{
	delete _sceneCamera;
}

#include "Debug/Logger/Logger.h"

void EditorScene::Init ()
{
	PerspectiveCamera* camera = (PerspectiveCamera*) _sceneCamera;

	camera->SetFieldOfView (45);

	std::string renderSettingsPath = "Assets/RenderSettings/Default.rsettings";

	_renderSettings = Resources::LoadRenderSettings (renderSettingsPath);
	RenderSettingsManager::Instance ()->AddRenderSettings (_renderSettings);
}

void EditorScene::Update ()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("Scene", NULL)) {

		ImGui::PopStyleVar();
		ImGui::Image ((void*)(intptr_t) _textureID, ImVec2 (_size.x, _size.y), ImVec2 (0, 1), ImVec2 (1, 0));

		ImVec2 position = ImGui::GetWindowPos ();

		ImVec2 limit1 = ImGui::GetWindowContentRegionMin ();
		ImVec2 limit2 = ImGui::GetWindowContentRegionMax ();

		_position = glm::ivec2 (position.x, position.y);
		_size = glm::ivec2 (limit2.x - limit1.x, limit2.y - limit1.y);

		_isHovered = ImGui::IsItemHovered ();

		_mousePosition = Input::GetMousePosition () - _position;
	}

	ImGui::End();

	_sceneCamera->SetAspect ((float) _size.x / _size.y);
}

void EditorScene::Render (const Scene* scene)
{
	_renderSettings->renderMode = "SceneRenderModule";
	_renderSettings->framebuffer.width = _size.x;
	_renderSettings->framebuffer.height = _size.y;
	_renderSettings->viewport.x = 0;
	_renderSettings->viewport.y = 0;
	_renderSettings->viewport.width = _size.x;
	_renderSettings->viewport.height = _size.y;

	RenderProduct result = RenderManager::Instance ()->RenderScene (scene, _sceneCamera, *_renderSettings);

	FrameBuffer2DVolume* framebuffer = dynamic_cast<FrameBuffer2DVolume*> (result.resultVolume);
	_textureID = framebuffer->GetColorTextureID ();
}

Camera* EditorScene::GetCamera ()
{
	return _sceneCamera;
}

glm::ivec2 EditorScene::GetWindowPosition () const
{
	return _position;
}

glm::ivec2 EditorScene::GetWindowSize () const
{
	return _size;
}

glm::ivec2 EditorScene::GetWindowMousePosition () const
{
	return _mousePosition;
}

bool EditorScene::IsWindowHovered () const
{
	return _isHovered;
}