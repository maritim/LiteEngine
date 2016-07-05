#include "Quint.h"

using namespace PennerEasing;

float Quint::easeIn (float t, float b, float c, float d) {
	t /= d;

	return c * t * t * t * t * t + b;
}
float Quint::easeOut (float t, float b, float c, float d) {
	t = t / d - 1;

	return c * (t * t * t * t * t + 1) + b;
}

float Quint::easeInOut (float t, float b, float c, float d) {
	t /= d / 2;

	if (t < 1) {
		return c / 2 * t * t * t * t * t + b;
	}

	t -= 2;

	return c / 2 * (t * t * t * t * t + 2) + b;
}