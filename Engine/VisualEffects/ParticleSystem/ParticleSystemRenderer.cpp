#include <GL/glew.h>

#include "ParticleSystemRenderer.h"

#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>

#include "Systems/Camera/Camera.h"

#include "Renderer/Pipeline.h"
#include "Texture/Texture.h"
#include "Texture/TextureAtlas.h"
#include "Managers/MaterialManager.h"
#include "Managers/TextureManager.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

void ParticleSystemRenderer::AddRenderer (ParticleRenderer* renderer)
{
	_particleRenderers.push_back (renderer);
}

void ParticleSystemRenderer::RemoveRenderer (ParticleRenderer* renderer)
{
	_toBeRemoved.insert (renderer);
}

void ParticleSystemRenderer::SetInstance (Particle* particle)
{
	_particleInstance = particle;
	this->Attach (particle->GetMesh ());
}

void ParticleSystemRenderer::SetParticlesCount (std::size_t partCount)
{
	_particlesCount = partCount;
}

void ParticleSystemRenderer::SetDepthMaskCheck (bool check)
{
	_useDepthMask = check;
}

void ParticleSystemRenderer::Draw ()
{
	RemoveMarked ();

	GLboolean depthMaskCheck;
	GL::GetBooleanv (GL_DEPTH_WRITEMASK, &depthMaskCheck); 
	if (!_useDepthMask) {
		GL::DepthMask ( GL_FALSE );

		InsertionSort ();
	} else {
		GL::DepthMask (GL_TRUE);
	}

	Pipeline::SetObjectTransform (_transform);

	Buffer<float> instancesBuffer;
	
	for (ParticleRenderer* particleRenderer : _particleRenderers) {
		instancesBuffer.Add (particleRenderer->GetParticleData ());
	}

	if (_particleRenderers.size () == 0) {
		return;
	}

	std::vector<PipelineAttribute> uniformAttributes = _particleRenderers [0]->GetUniformAttributes ();

	for (std::size_t i=0;i<_drawableObjects.size ();i++) {
		Material* mat = MaterialManager::Instance ()->GetMaterial (_drawableObjects [i].MAT_NAME);

		Pipeline::SendMaterial (mat);
		
		Pipeline::SendCustomAttributes (mat->shaderName, uniformAttributes);

		if (_drawableObjects [i].VBO_INSTANCE_INDEX == 0) {
			CreateVBO (_drawableObjects [i], _particleRenderers [0]->GetBufferAttributes ());
		}

		FeedVBO (_drawableObjects [i], instancesBuffer);

		//bind pe containerul de stare de geometrie (vertex array object)
		GL::BindVertexArray (_drawableObjects [i].VAO_INDEX);
		//comanda desenare
		GL::DrawElementsInstanced(GL_TRIANGLES, _drawableObjects [i].INDEX_COUNT, GL_UNSIGNED_INT, 0, _particleRenderers.size ());
	}
	
	GL::DepthMask ( depthMaskCheck );
}

void ParticleSystemRenderer::RemoveMarked ()
{
	std::set<ParticleRenderer*>::iterator it;
	for (std::size_t i=0;i<_particleRenderers.size ();) {
		it = _toBeRemoved.find (_particleRenderers [i]);
		if (it != _toBeRemoved.end ()) {
			_particleRenderers [i] = _particleRenderers.back ();
			_particleRenderers.pop_back ();
			_toBeRemoved.erase (it);

			continue;
		}

		i ++;
	}

	_toBeRemoved.clear ();
}

// The elements that will be sorted will be after
// one framed passed. This means that them don't
// change too much after the last sort, so will
// use a sorting algorithm that works better in
// almost sorted sets
// LE: Can improve this.
void ParticleSystemRenderer::InsertionSort ()
{
	glm::vec3 camPos = Camera::Main ()->GetPosition ();

	for (std::size_t i=1;i<_particleRenderers.size ();i++) {
		float dist = glm::distance2 (camPos, 
			_particleRenderers [i]->GetTransform ()->GetPosition ());

		for (std::size_t j=i-1;j>=0;j--) {
			float prevDist = glm::distance2 (camPos,
				_particleRenderers [j]->GetTransform ()->GetPosition ());

			if (dist <= prevDist) {
				break;
			}

			std::swap (_particleRenderers [j+1], _particleRenderers [j]);

			if (j == 0) {
				break;
			}
		}
	}
}

void ParticleSystemRenderer::CreateVBO (BufferObject& bufferObject, const std::vector<BufferAttribute>& bufferAttributes)
{
	GL::BindVertexArray(bufferObject.VAO_INDEX);

	GL::GenBuffers(1, &bufferObject.VBO_INSTANCE_INDEX);
	GL::BindBuffer(GL_ARRAY_BUFFER, bufferObject.VBO_INSTANCE_INDEX);
	GL::BufferData(GL_ARRAY_BUFFER, bufferAttributes [0].stride * _particlesCount, NULL, GL_DYNAMIC_DRAW);	

	for (BufferAttribute attr : bufferAttributes) {
		GL::EnableVertexAttribArray (attr.index);
		GL::VertexAttribPointer (attr.index, attr.size, attr.type, GL_FALSE, attr.stride, (void*) attr.pointer);
		GL::VertexAttribDivisor (attr.index, 1);
	}
}

void ParticleSystemRenderer::FeedVBO (BufferObject& bufferObject, const Buffer<float>& buffer)
{
	GL::BindVertexArray(bufferObject.VAO_INDEX);
	GL::BindBuffer(GL_ARRAY_BUFFER, bufferObject.VBO_INSTANCE_INDEX);
	GL::BufferSubData(GL_ARRAY_BUFFER, 0, buffer.GetBytesCount (), buffer.GetPointer ());
}