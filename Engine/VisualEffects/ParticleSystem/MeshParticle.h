#ifndef MESHPARTICLE_H
#define MESHPARTICLE_H

#include "Particle.h"

class MeshParticle : public Particle
{
protected:
	float _instanceBuffer [16];

public:
	MeshParticle ();

	std::vector<BufferAttribute> GetBufferAttributes () const;
	unsigned char* GetBuffer ();
	std::size_t GetSize () const;
	std::string GetShaderName () const { return ""; }
	std::vector<PipelineAttribute> GetAttributes () const;

	Particle* Clone ();
};

#endif