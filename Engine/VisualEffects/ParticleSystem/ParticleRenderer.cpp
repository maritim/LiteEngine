#include "ParticleRenderer.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

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

void ParticleRenderer::Attach (const Resource<Model>& model)
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

	glm::vec3 position = _transform->GetPosition ();
	glm::vec3 scalev = _transform->GetScale ();
	glm::quat rotationq = _transform->GetRotation ();

	glm::mat4 translate = glm::translate (glm::mat4 (1.f), glm::vec3 (position.x, position.y, position.z));
	glm::mat4 scale = glm::scale (glm::mat4 (1.f), glm::vec3 (scalev.x, scalev.y, scalev.z));
	glm::mat4 rotation = glm::mat4_cast(rotationq);

	glm::mat4 modelMatrix = translate * scale * rotation;

	for (short int i=0;i<4;i++) {
		for (short int j=0;j<4;j++) {
			buffer.Add (modelMatrix [i][j]);
		}
	}

	return buffer;
}