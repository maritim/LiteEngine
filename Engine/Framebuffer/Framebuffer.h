#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Core/Interfaces/Object.h"

#include <vector>

#include "Core/Resources/Resource.h"
#include "Texture/Texture.h"

#include "Core/Iteration/MultipleContainer.h"

class Framebuffer : public Object
{
protected:
	std::vector<Resource<Texture>> _textures;
	Resource<Texture> _depthTexture;

public:
	Framebuffer (const Resource<Texture>& texture,
		const Resource<Texture>& depthTexture = nullptr);
	Framebuffer (const std::vector<Resource<Texture>>& textures,
		const Resource<Texture>& depthTexture = nullptr);

	Resource<Texture> GetTexture (std::size_t index) const;
	Resource<Texture> GetDepthTexture () const;

	MULTIPLE_CONTAINER_TEMPLATE (vector)
};

MULTIPLE_CONTAINER_SPECIALIZATION (vector, Resource<Texture>, Framebuffer, _textures);

#endif