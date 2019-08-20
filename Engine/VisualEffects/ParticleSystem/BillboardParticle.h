#ifndef BILLBOARDPARTICLE_H
#define BILLBOARDPARTICLE_H

#include "Particle.h"

#include <string>

#include "Core/Resources/Resource.h"
#include "Texture/Texture.h"
#include "Renderer/RenderViews/TextureView.h"

class BillboardParticle : public Particle
{
protected:
	Resource<Texture> _textureAtlas;
	Resource<TextureView> _textureAtlasView;
	float _instanceBuffer [24];
	std::string _shaderName;

public:
	BillboardParticle ();

	void SetTextureAtlas (const Resource<Texture>& textureAtlas);

	std::vector<BufferAttribute> GetBufferAttributes () const;
	unsigned char* GetBuffer ();
	std::size_t GetSize () const;
	std::string GetShaderName () const;
	std::vector<PipelineAttribute> GetAttributes () const;

	Particle* Clone ();
};

#endif