#include "WeaponComponent.h"

#include "Systems/Camera/Camera.h"

#include "Managers/SceneManager.h"
#include "Resources/Resources.h"

#include "Utils/Extensions/MathExtend.h"

void WeaponComponent::Start ()
{
	_gun = SceneManager::Instance ()->Current ()->GetObject ("weapon");

	_offset = glm::vec3 (0.0f, -95.0f, 0.0f);
	_rotOffset = glm::quat (glm::vec3 (90.0f, 0, -180.0f) * DEG2RAD);
}

#include "Debug/Logger/Logger.h"

void WeaponComponent::Update ()
{
	Transform* gunTransform = _gun->GetTransform ();

	glm::vec3 camPos = Camera::Main ()->GetPosition ();
	glm::quat camRot = Camera::Main ()->GetRotation ();

	gunTransform->SetPosition (camPos + _offset);
	gunTransform->SetRotation (glm::conjugate (camRot) * _rotOffset);
}