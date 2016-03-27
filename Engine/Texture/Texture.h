#ifndef TEXTURE_H
#define TEXTURE_H

#include "Core/Interfaces/Object.h"

#include <SDL/SDL_image.h>

#include "Core/Math/glm/glm.hpp"

// TODO: Extend this
class Texture : public Object
{
protected:
	unsigned int _gpuIndex;
	std::string _name;
	SDL_Surface* _surface;

public:
	Texture(const std::string& name, unsigned int gpuIndex, 
		SDL_Surface* surface);
	Texture (const std::string& name, unsigned int gpuIndex);
	virtual ~Texture();

	unsigned int GetGPUIndex() const;
	std::string GetName() const;
	glm::uvec2 GetSize ();

	void SetGPUIndex(unsigned int gpuIndex);
	void SetName(const std::string& name);
	void SetSurface (SDL_Surface* surface);
};

#endif