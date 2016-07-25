#include "PointEmiter.h"

glm::vec3 PointEmiter::GetParticlePosition ()
{
	return this->GetTransform ()->GetPosition ();
}