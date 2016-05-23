#ifndef VECTORS_H
#define VECTORS_H

#include <assimp/scene.h>

#include "Core/Math/glm/glm.hpp"

namespace Conversions
{

glm::vec3 AssimpVectorToGLMVec (const aiVector4D& assimpVector);
aiVector4D GLMVecToAssimpVector (const glm::vec3& vec);

}

#endif