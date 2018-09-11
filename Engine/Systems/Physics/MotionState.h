#ifndef MOTIONSTATE_H
#define MOTIONSTATE_H

#include <LinearMath/btMotionState.h>

#include "SceneGraph/Transform.h"

class MotionState : public btMotionState
{
protected:
	Transform* _transform;
	glm::vec3 _offset;

public:
	MotionState (Transform* transform, const glm::vec3& offset);

	void getWorldTransform (btTransform& worldTransform) const;
	void setWorldTransform (const btTransform& worldTransform);
};

#endif