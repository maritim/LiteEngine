#ifndef POINTEMITER_H
#define POINTEMITER_H

#include "PrimitiveEmiter.h"

#include "Core/Math/Vector3.h"

class PointEmiter : public PrimitiveEmiter
{
public:
	using PrimitiveEmiter::PrimitiveEmiter;
protected:
	Vector3 GetParticlePosition ();
};

#endif