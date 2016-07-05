#include "MathExtend.h"

#include <cmath>

#define PI 3.14159265359

using namespace Extensions;

float MathExtend::Ctan (float x)
{
	return 1.0f / tan (x);
}

glm::mat4 MathExtend::Rotate (const glm::vec3& quat)
{
	glm::mat4 result (0.f);

	float cosa = cos (quat.x);
	float cosb = cos (quat.y);
	float cosc = cos (quat.z);

	float sina = sin (quat.x);
	float sinb = sin (quat.y);
	float sinc = sin (quat.z);

	result [0][0] = cosc * cosb;
	result [0][1] = sinc * cosa + sinb * sina * cosc;
	result [0][2] = sinc * sina - cosa * sinb * cosc;
	result [1][0] = -cosb * sinc;
	result [1][1] = cosa * cosc - sinc * sinb * sina;
	result [1][2] = sina * cosc + cosa * sinb * sinc;
	result [2][0] = sinb;
	result [2][1] = -sina * cosb;
	result [2][2] = cosa * cosb;
	result [3][3] = 1;

	return result;
}