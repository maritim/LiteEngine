#include "EditorScene.h"

#include <ctime>
#include <experimental/filesystem>
#include "Systems/GUI/ImGui/imgui.h"
#include "Systems/GUI/imguifilesystem/imguifilesystem.h"

#include "Renderer/RenderManager.h"
#include "Renderer/RenderSystem.h"

#include "RenderPasses/FrameBuffer2DVolume.h"

#include "Managers/SceneManager.h"
#include "Managers/RenderSettingsManager.h"

#include "Systems/Input/Input.h"
#include "Systems/Time/Time.h"

#include "Resources/Resources.h"

#include "Cameras/PerspectiveCamera.h"

#include "Utils/Files/FileSystem.h"

#include "Utils/Extensions/StringExtend.h"

namespace fs = std::experimental::filesystem;

EditorScene::EditorScene () :
	_sceneCamera (new PerspectiveCamera ()),
	_renderSettings (nullptr),
	_isActive (true),
	_position (0),
	_size (0),
	_textureID (0),
	_mousePosition (0),
	_isHovered (false),
	_targetFrameRate (-1)
{

}

EditorScene::~EditorScene ()
{
	delete _sceneCamera;
}

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
	if (ImGui::Begin("Scene", NULL, ImGuiWindowFlags_MenuBar)) {

		ImGui::PopStyleVar();

		ShowSceneMenu ();

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

void EditorScene::Render ()
{
	if (_isActive == false) {
		return;
	}

	if (_targetFrameRate != -1) {
		_elapsedFrameTime -= Time::GetDeltaTime ();

		if (_elapsedFrameTime > 0) {
			return;
		}
	}

	_elapsedFrameTime = _targetFrameRate == 0.0f ? 100 : 1.0f / _targetFrameRate;

	// _renderSettings->renderMode = "SceneRenderModule";
	_renderSettings->framebuffer.width = _size.x;
	_renderSettings->framebuffer.height = _size.y;
	_renderSettings->viewport.x = 0;
	_renderSettings->viewport.y = 0;
	_renderSettings->viewport.width = _size.x;
	_renderSettings->viewport.height = _size.y;

	RenderProduct result = RenderManager::Instance ()->Render (_sceneCamera, *_renderSettings);

	FrameBuffer2DVolume* framebuffer = dynamic_cast<FrameBuffer2DVolume*> (result.resultVolume);
	_textureID = framebuffer->GetColorTextureID ();
}

Camera* EditorScene::GetCamera ()
{
	return _sceneCamera;
}

void EditorScene::SetActive (bool isActive)
{
	_isActive = isActive;
}

bool EditorScene::IsActive () const
{
	return _isActive;
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

void EditorScene::ShowSceneMenu ()
{
	bool saveVolume = false;
	bool setFrameRate = false;

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Tools"))
        {
            saveVolume = ImGui::MenuItem("Take ScreenShot", "Ctrl+Shift+P");
            ImGui::Separator ();
			setFrameRate = ImGui::MenuItem("Set Frame Rate", "Ctrl+Shift+Y");
            if (ImGui::MenuItem("Set Resolution", "Ctrl+Shift+U")) { }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

	static ImGuiFs::Dialog dialog = ImGuiFs::Dialog ();

	std::time_t currentTime = std::time (nullptr);
	std::string filename = std::string (std::ctime (&currentTime));
	filename.pop_back ();
	Extensions::StringExtend::ReplaceAll (filename, ":", "-");
	filename += ".png";

	std::string volumePath = dialog.saveFileDialog(saveVolume, "", filename.c_str (), ".png");

	if (volumePath != std::string ()) {

		volumePath = FileSystem::Relative (volumePath, fs::current_path ().string ());

		Resource<TextureView> textureView (new TextureView (), "temp");
		textureView->SetGPUIndex (_textureID);

		Resource<Texture> texture = RenderSystem::SaveTexture (textureView);

		textureView->SetGPUIndex (0);

		Resources::SaveTexture (texture, volumePath);
	}

	if (setFrameRate) {
		ImGui::OpenPopup("FrameRate");
	}

	if (ImGui::BeginPopup("FrameRate")) {

		ImGui::InputFloat ("Frame Rate", &_targetFrameRate, 0, 0, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue);

		ImGui::EndPopup();
	}
}
