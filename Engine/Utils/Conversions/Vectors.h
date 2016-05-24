#ifndef VECTORS_H
#define VECTORS_H

#include <assimp/scene.h>

#include "Core/Math/glm/glm.hpp"

namespace Conversions
{

glm::vec3 AssimpVectorToGLMVec (const aiVector3D& assimpVector);
aiVector3D GLMVecToAssimpVector (const glm::vec3& vec);

}

#endif