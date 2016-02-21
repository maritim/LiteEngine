#ifndef PARTICLESYSTEMRENDERER_H
#define PARTICLESYSTEMRENDERER_H

#include "SceneNodes/Model3DRenderer.h"

#include <vector>
#include <set>

#include "ParticleRenderer.h"
#include "Particle.h"

#include "Material/Material.h"

#include "Renderer/BufferAttribute.h"

class ParticleSystemRenderer : public Model3DRenderer
{
protected:
	std::set<ParticleRenderer*> _toBeRemoved;
	std::vector<ParticleRenderer*> _particleRenderers;
	Particle* _particleInstance;
	std::size_t _particlesCount;
	bool _useDepthMask;

public:
	using Model3DRenderer::Model3DRenderer;

	void AddRenderer (ParticleRenderer* renderer);
	void RemoveRenderer (ParticleRenderer* renderer);

	void SetInstance (Particle* particle);
	void SetParticlesCount (std::size_t count);
	void SetDepthMaskCheck (bool check);

	void Draw ();
protected:
	void RemoveMarked ();
	void InsertionSort ();
	void FeedVBO (BufferObject& bufObject, const Buffer<float>& data); 
	void CreateVBO (BufferObject& bufObject, const std::vector<BufferAttribute>& attributes);
};

#endif