#include "Quad.h"

using namespace PennerEasing;

float Quad::easeIn (float t,float b , float c, float d) {
	t /= d;

	return c * t * t + b;
}
float Quad::easeOut(float t,float b , float c, float d) {
	t /= d;

	return -c * t * (t - 2) + b;
}

float Quad::easeInOut(float t,float b , float c, float d) {
	t /= d / 2;

	if (t < 1) {
		return ((c / 2) * (t * t)) + b;
	}

	-- t;

	return -c / 2 * (((t - 2) * t) - 1) + b;
}