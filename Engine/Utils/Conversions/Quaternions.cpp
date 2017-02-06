#include "Quaternions.h"

glm::quat Conversions::AssimpQuaternionToGLMQuat (const aiQuaternion& quaternion)
{
	glm::quat glmQuat;

	glmQuat.x = (float) quaternion.x;
	glmQuat.y = (float) quaternion.y;
	glmQuat.z = (float) quaternion.z;
	glmQuat.w = (float) quaternion.w;

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