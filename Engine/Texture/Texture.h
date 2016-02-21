#ifndef TEXTURE_H
#define TEXTURE_H

#include "Core/Interfaces/Object.h"

// TODO: Extend this
class Texture : public Object
{
protected:
	unsigned int _gpuIndex;
	std::string _name;
public:
	Texture(std::string name, unsigned int gpuIndex);
	virtual ~Texture();

	unsigned int GetGPUIndex() const;
	std::string GetName() const;

	void SetGPUIndex(unsigned int gpuIndex);
	void SetName(std::string name);
};

#endif