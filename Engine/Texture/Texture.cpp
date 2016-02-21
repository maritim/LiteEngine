#include "Texture.h"

#include <string>

Texture::Texture(std::string name, unsigned int gpuIndex)
{
	_name = name;
	_gpuIndex = gpuIndex;
}

Texture::~Texture()
{
	// TOOD: Free GPU memory
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

void Texture::SetName(std::string name)
{
	_name = name;
}