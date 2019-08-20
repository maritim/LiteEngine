#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "Texture.h"
#include <string>
#include <vector>

#include "Core/Resources/Resource.h"

class ENGINE_API CubeMap : public Texture
{
protected:
	std::vector<Resource<Texture>> _textures;

public:
	using Texture::Texture;

	void AddTexture (const Resource<Texture>& texture);
	Resource<Texture> GetTexture (std::size_t index) const;
};

#endif