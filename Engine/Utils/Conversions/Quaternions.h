#ifndef QUATERNIONS_H
#define QUATERNIONS_H

#include <assimp/scene.h>
#include <glm/gtc/quaternion.hpp>

namespace Conversions
{

glm::quat AssimpQuaternionToGLMQuat (const aiQuaternion& quaternion);
aiQuaternion GLMQuatToAssimpQuaternion (const glm::quat& quat);

}

#endif