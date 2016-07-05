#include "Circ.h"

using namespace PennerEasing;

float Circ::easeIn (float t, float b, float c, float d) {
	t /= d;

	return -c * (sqrt (1 - t * t) - 1) + b;
}

float Circ::easeOut (float t, float b, float c, float d) {
	t = t / d - 1;

	return c * sqrt(1 - t * t) + b;
}

float Circ::easeInOut (float t,float b , float c, float d) {
	t /= d / 2;

	if (t < 1) {
		return -c / 2 * (sqrt(1 - t*t) - 1) + b;
	}

	float postFix = t - 2;
	return c / 2 * (sqrt (1 - t * postFix) + 1) + b;
}