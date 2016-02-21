#ifndef QUADEMITER_H
#define QUADEMITER_H

#include "PrimitiveEmiter.h"

#include "Particle.h"

class QuadEmiter : public PrimitiveEmiter
{
protected:
	float _width;
	float _depth;

public:
	QuadEmiter (float width = 1.0f, float depth = 1.0f);

	void SetWidth (float width);
	void SetDepth (float depth);
protected:
	virtual Vector3 GetParticlePosition ();
};

#endif