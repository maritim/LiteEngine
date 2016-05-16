#include "Texture.h"

#include <string>

Texture::Texture(const std::string& name, SDL_Surface* surface) :
	_name (name),
	_gpuIndex (0),
	_surface (surface)
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

glm::uvec2 Texture::GetSize () const
{
	return glm::uvec2 (_surface->w, _surface->h);
}

std::string Texture::GetName() const
{
	return _name;
}

SDL_Surface* Texture::GetSurface () const
{
	return _surface;
}

void Texture::SetGPUIndex(unsigned int gpuIndex)
{
	_gpuIndex = gpuIndex;
}

void Texture::SetName(const std::string& name)
{
	_name = name;
}

void Texture::SetSurface (SDL_Surface* surface)
{
	_surface = surface;
}