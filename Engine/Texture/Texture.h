#ifndef TEXTURE_H
#define TEXTURE_H

#include "Core/Interfaces/Object.h"

#include <SDL2/SDL_image.h>

#include "Core/Math/glm/glm.hpp"

// TODO: Extend this
class Texture : public Object
{
protected:
	unsigned int _gpuIndex;
	std::string _name;
	SDL_Surface* _surface;

public:
	Texture(const std::string& name, SDL_Surface* surface);
	virtual ~Texture();

	unsigned int GetGPUIndex() const;
	std::string GetName() const;
	glm::uvec2 GetSize () const;
	SDL_Surface* GetSurface () const;

	void SetGPUIndex(unsigned int gpuIndex);
	void SetName(const std::string& name);
	void SetSurface (SDL_Surface* surface);
};

#endif