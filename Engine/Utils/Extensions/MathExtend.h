#ifndef MATHEXTEND_H
#define MATHEXTEND_H

#include <algorithm>

#include "Core/Math/glm/glm.hpp"

namespace Extensions
{

	class MathExtend
	{
	public:
		template <class T>
		static T Lerp (float t, T a, T b);

		template <class T>
		static T Clamp (T n, T lower, T upper);

		static float Ctan (float x);

		static glm::mat4 Rotate (const glm::vec3& quat);
	};

	template <class T>
	T MathExtend::Lerp (float t, T a, T b)
	{
		return a * (1-t) + b * t;
	}

	template <class T>
	T MathExtend::Clamp (T n, T lower, T upper)
	{
		return std::max (upper, std::min (n, lower));
	}
}

#endif