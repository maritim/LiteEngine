#ifndef POINTEMITER_H
#define POINTEMITER_H

#include "PrimitiveEmiter.h"

#include "Core/Math/glm/vec3.hpp"

class PointEmiter : public PrimitiveEmiter
{
public:
	using PrimitiveEmiter::PrimitiveEmiter;
protected:
	glm::vec3 GetParticlePosition ();
};

#endif