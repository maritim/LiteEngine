#include "EditorAnimationModel.h"

#include <sstream>
#include <iomanip>
#include <glm/vec2.hpp>
#include "Systems/GUI/ImGui/imgui.h"

#include "Renderer/Render/Mesh/AnimationModel.h"

// #include "SceneNodes/AnimationGameObject.h"

#include "Systems/Window/Window.h"

#include "Managers/SceneManager.h"

#include "Systems/Settings/SettingsManager.h"

EditorAnimationModel::EditorAnimationModel () :
	_currentAnimIndex (0),
	_isBlending (true),
	_blendDuration (1.0f)
{

}

void EditorAnimationModel::Show ()
{
	bool visible = SettingsManager::Instance ()->GetValue<bool> ("menu_show_animation_settings", false);

	if (visible == true) {
		ShowAnimationsWindow ();
	}
}

void EditorAnimationModel::ShowAnimationsWindow ()
{
	std::size_t windowHeight = Window::GetHeight ();

	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(350, windowHeight - 10), ImGuiCond_Always);

	ImGuiWindowFlags windowFlags = 0;

	if (!ImGui::Begin("Animations", NULL, windowFlags)) {
		ImGui::End();

		return;
	}

	// for (SceneObject* object : *SceneManager::Instance ()->Current ()) {

		// AnimationGameObject* animGameObject = dynamic_cast<AnimationGameObject*> (object);

		// if (animGameObject == nullptr) {
		// 	continue;
		// }

		// std::string objectName = animGameObject->GetName ();

		// if (ImGui::CollapsingHeader (std::string ("Model: " + objectName).c_str ())) {

			// Resource<Model> model = animGameObject->GetMesh ();
			// AnimationModel* animModel = (AnimationModel*) &*model;
			// AnimationsController* animController = animModel->GetAnimationsController ();

			// std::vector<std::string> animations = { "None" };
			// std::string items = std::string ("None") + '\0';

			// for (AnimationContainer* animContainer : *animController) {
			// 	animations.push_back (animContainer->GetName ());
			// 	items += animContainer->GetName () + '\0';
			// }

			// int lastAnimIndex = _currentAnimIndex;

			// ImGui::Combo("Animation", &_currentAnimIndex, items.c_str ());

			// AnimationContainer* animContainer = animController->GetAnimationContainer (animations [_currentAnimIndex]);

			// ImGui::LabelText ("Duration", "%.3f", animContainer != nullptr ? animContainer->GetDuration () : 0.0f);

			// ImGui::Spacing ();

			// ImGui::Checkbox ("Blending", &_isBlending);

			// ImGui::InputFloat ("Blend Duration", &_blendDuration);

			// if (lastAnimIndex != _currentAnimIndex) {
			// 	if (_isBlending) {
			// 		animGameObject->Blend (animations [_currentAnimIndex], _blendDuration);
			// 	}

			// 	if (!_isBlending) {
			// 		animGameObject->SetAnimationClip (animations [_currentAnimIndex]);
			// 	}
			// }
		// }
	// }

	ImGui::End();
}

