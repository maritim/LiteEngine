#include "EditorGame.h"
#include "EditorScene.h"

#include <ctime>
#include <filesystem>
#include <ImGui/imgui.h>
#include <ImGui/imguifilesystem/imguifilesystem.h>

#include "Renderer/RenderManager.h"
#include "Renderer/RenderSystem.h"

#include "RenderPasses/FramebufferRenderVolume.h"

#include "Managers/SceneManager.h"
#include "Managers/RenderSettingsManager.h"

#include "Systems/Input/Input.h"
#include "Systems/Time/Time.h"
#include "Systems/Settings/SettingsManager.h"

#include "Resources/Resources.h"

#include "Audio/AudioSource.h"

#include "Cameras/PerspectiveCamera.h"

#include "Components/Cameras/PerspectiveCameraComponent.h"


#include "Utils/Files/FileSystem.h"

#include "Utils/Extensions/StringExtend.h"

namespace fs = std::filesystem;

EditorGame::EditorGame():
	_sceneCamera(new PerspectiveCamera),
	_renderSettings(nullptr),
	_isActive(true),
	_position(0),
	_size(0),
	_textureID(0),
	_mousePosition(0),
	_isHovered(false),
	_targetFrameRate(-1)
{

}

EditorGame::~EditorGame()
{
	delete _sceneCamera;
}

void EditorGame::Init()
{
	_sceneCamera->SetZNear(0.3f);
	_sceneCamera->SetZFar(100.0f);
	_sceneCamera->SetConstrainAspect(true);

	PerspectiveCamera* camera = (PerspectiveCamera*)_sceneCamera;

	camera->SetFieldOfViewAngle(75);

	std::string renderSettingsPath = "Assets/RenderSettings/Default.rsettings";

	_renderSettings = Resources::LoadRenderSettings(renderSettingsPath);
	RenderSettingsManager::Instance()->AddRenderSettings(_renderSettings);
}

void EditorGame::Update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("Game", NULL, ImGuiWindowFlags_MenuBar)) {

		if (_sceneCamera != NULL)
		{

			ImGui::PopStyleVar();

			ShowSceneMenu();

			ImGui::Image((void*)(intptr_t)_textureID, ImVec2(_size.x, _size.y), ImVec2(0, 1), ImVec2(1, 0));

			ImVec2 position = ImGui::GetWindowPos();

			ImVec2 limit1 = ImGui::GetWindowContentRegionMin();
			ImVec2 limit2 = ImGui::GetWindowContentRegionMax();

			_position = glm::ivec2(position.x, position.y);
			_size = glm::ivec2(limit2.x - limit1.x, limit2.y - limit1.y);

			_isHovered = ImGui::IsItemHovered();

			_mousePosition = Input::GetMousePosition() - _position;
		}
		else
		{
			ImGui::Text("No camera rendering on this scene");
			ImGui::Text("Create a perspective or orthographic camera");
		}
	}

	ImGui::End();

}

void EditorGame::Render()
{

	if (_sceneCamera != NULL)
	{
		if (_isActive == false) {
			return;
		}

		if (_targetFrameRate != -1) {
			_elapsedFrameTime -= Time::GetDeltaTime();

			if (_elapsedFrameTime > 0) {
				return;
			}
		}
		_elapsedFrameTime = _targetFrameRate == 0.0f ? 100 : 1.0f / _targetFrameRate;

		// _renderSettings->renderMode = "SceneRenderModule";
		_renderSettings->resolution.width = _size.x;
		_renderSettings->resolution.height = _size.y;
		_renderSettings->viewport.x = 0;
		_renderSettings->viewport.y = 0;
		_renderSettings->viewport.width = _size.x;
		_renderSettings->viewport.height = _size.y;

		RenderProduct result = RenderManager::Instance()->Render(EditorScene::Instance()->GetCamera(), *_renderSettings);

		FramebufferRenderVolume* renderVolume = dynamic_cast<FramebufferRenderVolume*> (result.resultVolume);
		_textureID = renderVolume->GetFramebufferView()->GetTextureView(0)->GetGPUIndex();
	}
}

Camera* EditorGame::GetCamera()
{
	return _sceneCamera;
}

void EditorGame::SetActive(bool isActive)
{
	_isActive = isActive;
}

bool EditorGame::IsActive() const
{
	return _isActive;
}

glm::ivec2 EditorGame::GetWindowPosition() const
{
	return _position;
}

glm::ivec2 EditorGame::GetWindowSize() const
{
	return _size;
}

glm::ivec2 EditorGame::GetWindowMousePosition() const
{
	return _mousePosition;
}

bool EditorGame::IsWindowHovered() const
{
	return _isHovered;
}

void EditorGame::ShowSceneMenu()
{
	bool saveVolume = false;
	bool setFrameRate = false;

	// Menu Bar
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Tools"))
		{
			saveVolume = ImGui::MenuItem("Take ScreenShot", "Ctrl+Shift+P");
			ImGui::Separator();
			setFrameRate = ImGui::MenuItem("Set Frame Rate", "Ctrl+Shift+Y");
			if (ImGui::MenuItem("Set Resolution", "Ctrl+Shift+U")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Audio"))
		{
			ImGui::MenuItem("Mute audio");
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	static ImGuiFs::Dialog dialog = ImGuiFs::Dialog();

	std::time_t currentTime = std::time(nullptr);
	std::string filename = std::string(std::ctime(&currentTime));
	filename.pop_back();
	Extensions::StringExtend::ReplaceAll(filename, ":", "-");
	filename += ".png";

	std::string volumePath = dialog.saveFileDialog(saveVolume, "", filename.c_str(), ".png");

	if (volumePath != std::string()) {

		volumePath = FileSystem::Relative(volumePath, fs::current_path().string());

		Resource<TextureView> textureView(new TextureView(), "temp");
		textureView->SetGPUIndex(_textureID);

		Resource<Texture> texture = RenderSystem::SaveTexture(textureView);

		textureView->SetGPUIndex(0);

		Resources::SaveTexture(texture, volumePath);
	}

	if (setFrameRate) {
		ImGui::OpenPopup("FrameRate");
	}

	if (ImGui::BeginPopup("FrameRate")) {

		ImGui::InputFloat("Frame Rate", &_targetFrameRate, 0, 0, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue);

		ImGui::EndPopup();
	}
}

void EditorGame::ShowNoCameraMenu()
{
	if (ImGui::BeginMenuBar())
	{
		ImGui::Text("No camera rendering, create a perspective or orthographic camera");
	}
}
