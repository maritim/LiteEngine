#include "EaseCurve.h"

#include "PennerEasing/Linear.h"
#include "PennerEasing/Quad.h"
#include "PennerEasing/Cubic.h"
#include "PennerEasing/Sine.h"
#include "PennerEasing/Quint.h"
#include "PennerEasing/Back.h"
#include "PennerEasing/Bounce.h"
#include "PennerEasing/Circ.h"
#include "PennerEasing/Elastic.h"
#include "PennerEasing/Expo.h"
#include "PennerEasing/Quart.h"

float EaseCurve::Evaluate (EaseType type, float t)
{
	float result = 0;

	switch (type) {
		case ZERO: result = 0; break;
		case ONE: result = 1; break;
		case LINEAR: result = PennerEasing::Linear::easeNone (t, 0, 1, 1); break;
		case QUAD_EASE_IN: result = PennerEasing::Quad::easeIn (t, 0, 1, 1); break;
		case QUAD_EASE_OUT: result = PennerEasing::Quad::easeOut (t, 0, 1, 1); break;
		case QUAD_EASE_IN_OUT: result = PennerEasing::Quad::easeInOut (t, 0, 1, 1); break;
		case CUBIC_EASE_IN: result = PennerEasing::Cubic::easeIn (t, 0, 1, 1); break;
		case CUBIC_EASE_OUT: result = PennerEasing::Cubic::easeOut (t, 0, 1, 1); break;
		case CUBIC_EASE_IN_OUT: result = PennerEasing::Cubic::easeInOut (t, 0, 1, 1); break;
		case SINE_EASE_IN: result = PennerEasing::Sine::easeIn (t, 0, 1, 1); break;
		case SINE_EASE_OUT: result = PennerEasing::Sine::easeOut (t, 0, 1, 1); break;
		case SINE_EASE_IN_OUT: result = PennerEasing::Sine::easeInOut (t, 0, 1, 1); break;
		case QUINT_EASE_IN: result = PennerEasing::Quint::easeIn (t, 0, 1, 1); break;
		case QUINT_EASE_OUT: result = PennerEasing::Quint::easeOut (t, 0, 1, 1); break;
		case QUINT_EASE_IN_OUT: result = PennerEasing::Quint::easeInOut (t, 0, 1, 1); break;
		case BACK_EASE_IN: result = PennerEasing::Back::easeIn (t, 0, 1, 1); break;
		case BACK_EASE_OUT: result = PennerEasing::Back::easeOut (t, 0, 1, 1); break;
		case BACK_EASE_IN_OUT: result = PennerEasing::Back::easeInOut (t, 0, 1, 1); break;
		case BOUNCE_EASE_IN: result = PennerEasing::Bounce::easeIn (t, 0, 1, 1); break;
		case BOUNCE_EASE_OUT: result = PennerEasing::Bounce::easeOut (t, 0, 1, 1); break;
		case BOUNCE_EASE_IN_OUT: result = PennerEasing::Bounce::easeInOut (t, 0, 1, 1); break;
		case CIRC_EASE_IN: result = PennerEasing::Circ::easeIn (t, 0, 1, 1); break;
		case CIRC_EASE_OUT: result = PennerEasing::Circ::easeOut (t, 0, 1, 1); break;
		case CIRC_EASE_IN_OUT: result = PennerEasing::Circ::easeInOut (t, 0, 1, 1); break;
		case ELASTIC_EASE_IN: result = PennerEasing::Elastic::easeIn (t, 0, 1, 1); break;
		case ELASTIC_EASE_OUT: result = PennerEasing::Elastic::easeOut (t, 0, 1, 1); break;
		case ELASTIC_EASE_IN_OUT: result = PennerEasing::Elastic::easeInOut (t, 0, 1, 1); break;
		case EXPO_EASE_IN: result = PennerEasing::Expo::easeIn (t, 0, 1, 1); break;
		case EXPO_EASE_OUT: result = PennerEasing::Expo::easeOut (t, 0, 1, 1); break;
		case EXPO_EASE_IN_OUT: result = PennerEasing::Expo::easeInOut (t, 0, 1, 1); break;
		case QUART_EASE_IN: result = PennerEasing::Quart::easeIn (t, 0, 1, 1); break;
		case QUART_EASE_OUT: result = PennerEasing::Quart::easeOut (t, 0, 1, 1); break;
		case QUART_EASE_IN_OUT: result = PennerEasing::Quart::easeInOut (t, 0, 1, 1); break;
	}

	return result;
}