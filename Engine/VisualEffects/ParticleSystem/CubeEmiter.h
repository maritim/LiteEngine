#ifndef CUBEEMITER_H
#define CUBEEMITER_H

#include "QuadEmiter.h"

#include "Core/Math/Vector3.h"

class CubeEmiter : public QuadEmiter
{
private:
	float _height;

public:
	CubeEmiter (float width = 1.0f, float depth = 1.0f, float height = 1.0f);

	void SetHeight (float height);
protected:
	Vector3 GetParticlePosition ();
};

#endif