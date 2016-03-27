#include "Texture.h"

#include <string>

Texture::Texture(const std::string& name, unsigned int gpuIndex,
	SDL_Surface* surface) :
	_name (name),
	_gpuIndex (gpuIndex),
	_surface (surface)
{

}

Texture::Texture (const std::string& name, unsigned int gpuIndex) :
	_name (name),
	_gpuIndex (gpuIndex),
	_surface (nullptr)
{

}

Texture::~Texture()
{
	SDL_FreeSurface (_surface);
}

unsigned int Texture::GetGPUIndex() const
{
	return _gpuIndex;
}

std::string Texture::GetName() const
{
	return _name;
}

void Texture::SetGPUIndex(unsigned int gpuIndex)
{
	_gpuIndex = gpuIndex;
}

void Texture::SetName(const std::string& name)
{
	_name = name;
}

glm::uvec2 Texture::GetSize ()
{
	return glm::uvec2 (_surface->w, _surface->h);
}

void Texture::SetSurface (SDL_Surface* surface)
{
	_surface = surface;
}