#ifndef MATHEXTEND_H
#define MATHEXTEND_H

#include <algorithm>

#define PI 3.14159265359
#define F_PI 3.14159f

#define DEG2RAD (float)(F_PI / 180.0f)

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