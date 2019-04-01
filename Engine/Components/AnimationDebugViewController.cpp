#include "AnimationDebugViewController.h"

#include <sstream>
#include <iomanip>
#include <glm/vec2.hpp>
#include "Editor/ImGui/imgui.h"

#include "Mesh/AnimationModel.h"

#include "SceneNodes/AnimationGameObject.h"
#include "SceneNodes/AnimationModel3DRenderer.h"

#include "Systems/Window/Window.h"

#include "Managers/SceneManager.h"

void AnimationDebugViewController::Start ()
{
	_currentAnimIndex = 0;
	_isBlending = true;
	_blendDuration = 1.0f;
}

void AnimationDebugViewController::Update ()
{
	ShowAnimationsWindow ();
}

void AnimationDebugViewController::ShowAnimationsWindow ()
{
	std::size_t windowHeight = Window::GetHeight ();

	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(350, windowHeight - 10), ImGuiCond_Always);

	ImGuiWindowFlags windowFlags = 0;

	if (!ImGui::Begin("Animations", NULL, windowFlags)) {
		ImGui::End();

		return;
	}

	for (SceneObject* object : *SceneManager::Instance ()->Current ()) {

		AnimationGameObject* animGameObject = dynamic_cast<AnimationGameObject*> (object);

		if (animGameObject == nullptr) {
			continue;
		}

		std::string objectName = animGameObject->GetName ();

		if (ImGui::CollapsingHeader (std::string ("Model: " + objectName).c_str ())) {

			AnimationModel* animModel = (AnimationModel*) animGameObject->GetMesh ();
			AnimationsController* animController = animModel->GetAnimationsController ();

			std::vector<std::string> animations = { "None" };
			std::string items = std::string ("None") + '\0';

			for (AnimationContainer* animContainer : *animController) {
				animations.push_back (animContainer->GetName ());
				items += animContainer->GetName () + '\0';
			}

			int lastAnimIndex = _currentAnimIndex;

			ImGui::Combo("Animation", &_currentAnimIndex, items.c_str ());

			AnimationContainer* animContainer = animController->GetAnimationContainer (animations [_currentAnimIndex]);

			ImGui::LabelText ("Duration", "%.3f", animContainer != nullptr ? animContainer->GetDuration () : 0.0f);

			ImGui::Spacing ();

			ImGui::Checkbox ("Blending", &_isBlending);

			ImGui::InputFloat ("Blend Duration", &_blendDuration);

			if (lastAnimIndex != _currentAnimIndex) {
				AnimationModel3DRenderer* renderer = (AnimationModel3DRenderer*) animGameObject->GetRenderer ();

				if (_isBlending) {
					renderer->Blend (animations [_currentAnimIndex], _blendDuration);
				}

				if (!_isBlending) {
					renderer->SetAnimationClip (animations [_currentAnimIndex]);
				}
			}
		}
	}

	ImGui::End();
}
