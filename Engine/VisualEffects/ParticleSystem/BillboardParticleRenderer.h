#ifndef BILLBOARDPARTICLERENDERER_H
#define BILLBOARDPARTICLERENDERER_H

#include "ParticleRenderer.h"

#include <string>

#include "Material/Material.h"
#include "Mesh/Model.h"

#include "Renderer/Buffer.h"

class BillboardParticleRenderer : public ParticleRenderer
{
private:
	unsigned int* _lifetime;
	unsigned int* _timeAlive;
	std::string _matName;

public:
	using ParticleRenderer::ParticleRenderer;

	Buffer<float> GetParticleData ();
	std::vector<BufferAttribute> GetBufferAttributes ();
	std::vector<PipelineAttribute> GetUniformAttributes ();

	void SetLifetime (unsigned int* lifetime);
	void SetTimeAlive (unsigned int* timeAlive);

	void Attach (Model* mesh);
private:
	void ManageAtlasTexcoord (Material* mat, Buffer<float>* data);
	void ManageCustomAttributes (Buffer<float>* data);
};

#endif