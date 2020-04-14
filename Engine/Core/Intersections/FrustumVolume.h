#ifndef FRUSTUMVOLUME_H
#define FRUSTUMVOLUME_H

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

struct ENGINE_API FrustumVolume
{
	const static int PLANESCOUNT = 6;

	glm::vec4 plane [PLANESCOUNT];

	FrustumVolume (const glm::mat4& mvpMatrix)
	{
		glm::mat4 MVP = glm::transpose (mvpMatrix);

		plane [0].x = MVP [0][0] + MVP [3][0];
		plane [0].y = MVP [0][1] + MVP [3][1];
		plane [0].z = MVP [0][2] + MVP [3][2];
		plane [0].w = MVP [0][3] + MVP [3][3];

		plane [1].x = - MVP [0][0] + MVP [3][0];
		plane [1].y = - MVP [0][1] + MVP [3][1];
		plane [1].z = - MVP [0][2] + MVP [3][2];
		plane [1].w = - MVP [0][3] + MVP [3][3];

		plane [2].x = MVP [1][0] + MVP [3][0];
		plane [2].y = MVP [1][1] + MVP [3][1];
		plane [2].z = MVP [1][2] + MVP [3][2];
		plane [2].w = MVP [1][3] + MVP [3][3];

		plane [3].x = - MVP [1][0] + MVP [3][0];
		plane [3].y = - MVP [1][1] + MVP [3][1];
		plane [3].z = - MVP [1][2] + MVP [3][2];
		plane [3].w = - MVP [1][3] + MVP [3][3];

		plane [4].x = MVP [2][0] + MVP [3][0];
		plane [4].y = MVP [2][1] + MVP [3][1];
		plane [4].z = MVP [2][2] + MVP [3][2];
		plane [4].w = MVP [2][3] + MVP [3][3];

		plane [5].x = - MVP [2][0] + MVP [3][0];
		plane [5].y = - MVP [2][1] + MVP [3][1];
		plane [5].z = - MVP [2][2] + MVP [3][2];
		plane [5].w = - MVP [2][3] + MVP [3][3];

		for (std::size_t i=0;i<6;i++) {
			plane [i] = glm::normalize (plane [i]);
		}
	}
};

#endif