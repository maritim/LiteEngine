#include "MeshParticleRenderer.h"

MeshParticleRenderer::~MeshParticleRenderer ()
{
	// Keep nothing, do nothing
}

void MeshParticleRenderer::Attach (const Resource<Model>& mesh)
{
	// Do nothing
}

Buffer<float> MeshParticleRenderer::GetParticleData ()
{
	Buffer<float> buffer;

	buffer.Add (this->GetModelMatrix ());

	return buffer;
}

std::vector<PipelineAttribute> MeshParticleRenderer::GetUniformAttributes ()
{
	std::vector<PipelineAttribute> attributes;

	return attributes;
}

std::vector<BufferAttribute> MeshParticleRenderer::GetBufferAttributes ()
{
	std::vector<BufferAttribute> attributes;

	for (int i=0;i<4;i++) {
		BufferAttribute attribute;

		attribute.index = 3 + i;
		attribute.size = 4;
		attribute.type = GL_FLOAT;
		attribute.stride = 16 * sizeof (float);
		attribute.pointer = i * 4 * sizeof (float);

		attributes.push_back (attribute);
	}

	return attributes;
}