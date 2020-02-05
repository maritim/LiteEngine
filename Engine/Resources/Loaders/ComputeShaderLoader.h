#ifndef COMPUTESHADERLOADER_H
#define COMPUTESHADERLOADER_H

#include "Resources/ResourceLoader.h"

class ComputeShaderLoader : public ResourceLoader
{
public:
	Object* Load(const std::string& filename);
};

#endif