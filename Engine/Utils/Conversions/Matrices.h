#ifndef MATRICES_H
#define MATRICES_H

#include <assimp/scene.h>

#include "Core/Math/glm/glm.hpp"

namespace Conversions
{

glm::mat4 AssimpMatrixToGLMMat (const aiMatrix4x4& matrix);
aiMatrix4x4 GLMMatToAssimpMatrix (const glm::mat4& matrix);

}

#endif