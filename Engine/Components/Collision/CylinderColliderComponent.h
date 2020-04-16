#ifndef CYLINDERCOLLIDERCOMPONENT_H
#define CYLINDERCOLLIDERCOMPONENT_H

#include "BoxColliderComponent.h"

class ENGINE_API CylinderColliderComponent : public BoxColliderComponent
{
	DECLARE_COMPONENT(CylinderColliderComponent)

public:
	void SetExtents (const glm::vec3& extents);
};

#endif