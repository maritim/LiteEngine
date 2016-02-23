#ifndef MATHEXTEND_H
#define MATHEXTEND_H

namespace Extensions
{

class MathExtend
{
	public:
		template <class T>
		static T Lerp (float t, T a, T b);
	};

	template <class T>
	T MathExtend::Lerp (float t, T a, T b)
	{
		return a * (1-t) + b * t;
	}

}

#endif