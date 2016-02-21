#include "PointEmiter.h"

Vector3 PointEmiter::GetParticlePosition ()
{
	return this->GetTransform ()->GetPosition ();
}