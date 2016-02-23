#include "AnimationCurve.h"

#include "EaseCurve.h"

AnimationCurve::AnimationCurve (EaseCurve::EaseType type) :
	_easeType (type)
{

}

AnimationCurve::~AnimationCurve ()
{

}

void AnimationCurve::SetEaseType (EaseCurve::EaseType type)
{
	_easeType = type;
}

float AnimationCurve::Evaluate (float t)
{
	return EaseCurve::Evaluate (_easeType, t);
}