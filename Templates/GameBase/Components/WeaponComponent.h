#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H

#include "Systems/Components/Component.h"

#include <glm/vec3.hpp>

#include "SceneGraph/SceneObject.h"

class WeaponComponent : public Component
{
	DECLARE_COMPONENT(WeaponComponent)

private:
	SceneObject* _gun;
	glm::vec3 _offset;
	glm::quat _rotOffset;

public:
	void Start ();

	void Update ();
};

REGISTER_COMPONENT (WeaponComponent)

#endif