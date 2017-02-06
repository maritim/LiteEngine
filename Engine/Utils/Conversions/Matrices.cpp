#include "Matrices.h"

glm::mat4 Conversions::AssimpMatrixToGLMMat (const aiMatrix4x4& matrix)
{
	glm::mat4 glmMatrix;

	for (glm::length_t i=0;i<4;i++) {
		for (glm::length_t j=0;j<4;j++) {
			glmMatrix [j][i] = (float) matrix [i][j];
		}
	}

	return glmMatrix;
}

aiMatrix4x4 Conversions::GLMMatToAssimpMatrix (const glm::mat4& matrix)
{
	aiMatrix4x4 assimpMatrix;

	for (glm::length_t i=0;i<4;i++) {
		for (glm::length_t j=0;j<4;j++) {
			assimpMatrix [i][j] = (float) matrix [j][i];
		}
	}

	return assimpMatrix;
}