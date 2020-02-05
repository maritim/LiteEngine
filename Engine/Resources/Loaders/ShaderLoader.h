#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include "Resources/ResourceLoader.h"

#include <vector>

class ShaderLoader : public ResourceLoader
{
protected:
	std::vector<std::string> _filenames;

public:
	Object* Load(const std::string& filename);

	void SetFilenames(const std::vector<std::string>& filenames);
};

#endif