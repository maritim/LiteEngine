#include "Vectors.h"

glm::vec3 Conversions::AssimpVectorToGLMVec (const aiVector3D& assimpVector)
{
	glm::vec3 vec;

	vec.x = assimpVector.x;
	vec.y = assimpVector.y;
	vec.z = assimpVector.z;

	return vec;
}

aiVector3D Conversions::GLMVecToAssimpVector (const glm::vec3& vec)
{
	aiVector3D assimpVector;

	assimpVector.x = vec.x;
	assimpVector.y = vec.y;
	assimpVector.z = vec.z;

	return assimpVector;
}
