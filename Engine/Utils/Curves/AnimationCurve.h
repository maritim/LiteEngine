#ifndef ANIMATIONCURVE_H
#define ANIMATIONCURVE_H

#include "Core/Interfaces/Object.h"

#include "EaseCurve.h"

class AnimationCurve : public Object 
{
private:
	EaseCurve::EaseType _easeType;

public:
	AnimationCurve (EaseCurve::EaseType type = EaseCurve::EaseType::LINEAR);
	~AnimationCurve ();

	void SetEaseType (EaseCurve::EaseType type);

	float Evaluate (float t);
};

#endif