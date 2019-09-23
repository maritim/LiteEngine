#ifndef CUBEMAPLOADER_H
#define CUBEMAPLOADER_H

#include "Resources/ResourceLoader.h"

#include <string>
#include <vector>

class CubeMapLoader : public ResourceLoader
{
private:
	std::vector<std::string> _filenames;
public:
	Object* Load(const std::string& filename);

	void SetFilenames(std::vector<std::string> filenames);
};

#endif