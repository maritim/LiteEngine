#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include "Resources/ResourceLoader.h"

#include <string>

class TextureLoader : public ResourceLoader
{
public:
	Object* Load(const std::string& filename);
};

#endif