#ifndef PARTICLERENDERER_H
#define PARTICLERENDERER_H

#include "SceneNodes/Model3DRenderer.h"

#include "Renderer/Buffer.h"
#include "Renderer/BufferAttribute.h"
#include "Renderer/PipelineAttribute.h"

class ParticleRenderer : public Model3DRenderer
{
public:
	using Model3DRenderer::Model3DRenderer;

	virtual Buffer<float> GetParticleData ();
	virtual std::vector<PipelineAttribute> GetUniformAttributes ();
	virtual std::vector<BufferAttribute> GetBufferAttributes ();

	virtual ~ParticleRenderer ();

	virtual void Attach (Model* mesh);

	void Draw ();
protected:
	Buffer<float> GetModelMatrix ();
};

#endif