#include "CubeMap.h"
#include "Texture.h"

void CubeMap::AddTexture (const Resource<Texture>& texture)
{
	_textures.push_back (texture);
}

Resource<Texture> CubeMap::GetTexture (std::size_t index) const
{
	return _textures [index];
}
