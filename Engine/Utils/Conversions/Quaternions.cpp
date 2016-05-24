#include "Quaternions.h"

glm::quat Conversions::AssimpQuaternionToGLMQuat (const aiQuaternion& quaternion)
{
	glm::quat glmQuat;

	glmQuat.x = quaternion.x;
	glmQuat.y = quaternion.y;
	glmQuat.z = quaternion.z;
	glmQuat.w = quaternion.w;

	return glmQuat;
}

aiQuaternion GLMQuatToAssimpQuaternion (const glm::quat& quat)
{
	aiQuaternion assimpQuaternion;

	assimpQuaternion.x = quat.x;
	assimpQuaternion.y = quat.y;
	assimpQuaternion.z = quat.z;
	assimpQuaternion.w = quat.w;

	return assimpQuaternion;
}