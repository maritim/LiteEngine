#include "ParticleRenderer.h"

#include <string>

#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"

ParticleRenderer::~ParticleRenderer ()
{
	
}

Buffer<float> ParticleRenderer::GetParticleData ()
{
	Buffer<float> data;

	return data;
}

std::vector<BufferAttribute> ParticleRenderer::GetBufferAttributes ()
{
	std::vector<BufferAttribute> attributes;

	return attributes;
}

std::vector<PipelineAttribute> ParticleRenderer::GetUniformAttributes ()
{
	std::vector<PipelineAttribute> attributes;

	return attributes;
}

void ParticleRenderer::Attach (Model* model)
{
	// Do nothing
}

void ParticleRenderer::Draw ()
{
	// Do nothing. Don't have to implement it
}

Buffer<float> ParticleRenderer::GetModelMatrix ()
{
	Buffer<float> buffer;

	Vector3 position = _transform->GetPosition ();
	Vector3 scalev = _transform->GetScale ();
	Vector3 rotationv = _transform->GetRotation ();

	Matrix translate = Matrix::Translate (position.x, position.y, position.z);
	Matrix scale = Matrix::Scale (scalev.x, scalev.y, scalev.z);
	Matrix rotation = Matrix::Rotate (rotationv.x, rotationv.y, rotationv.z);

	Matrix modelMatrix = translate * scale * rotation;

	float* cMatrix = modelMatrix.C_ColMatrix ();
	for (std::size_t i=0;i<16;i++) {
		buffer.Add (cMatrix [i]);
	}
	delete[] cMatrix;

	return buffer;
}