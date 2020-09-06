#include "Framebuffer.h"

#include "Core/Console/Console.h"

Framebuffer::Framebuffer (const Resource<Texture>& texture,
	const Resource<Texture>& depthTexture) :
	_depthTexture (depthTexture)
{
	if (texture != nullptr) {
		_textures.push_back (texture);
	}
}

Framebuffer::Framebuffer (const std::vector<Resource<Texture>>& textures,
	const Resource<Texture>& depthTexture) :
	_textures (textures),
	_depthTexture (depthTexture)
{

}

Resource<Texture> Framebuffer::GetTexture (std::size_t index) const
{
	if (index >= _textures.size ()) {
		Console::LogWarning ("There is no texture index " + std::to_string (index) + " in framebuffer.");

		return nullptr;
	}

	return _textures [index];
}

Resource<Texture> Framebuffer::GetDepthTexture () const
{
	return _depthTexture;
}
