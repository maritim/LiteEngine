#ifndef MESHPARTICLERENDERER_H
#define MESHPARTICLERENDERER_H

#include "ParticleRenderer.h"

#include "Mesh/Model.h"

class MeshParticleRenderer : public ParticleRenderer
{
public:
	using ParticleRenderer::ParticleRenderer;

	virtual ~MeshParticleRenderer ();

	virtual Buffer<float> GetParticleData ();

	virtual std::vector<PipelineAttribute> GetUniformAttributes ();
	virtual std::vector<BufferAttribute> GetBufferAttributes ();

	virtual void Attach (Model* mesh);
};

#endif