#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include "ResourceLoader.h"

#include <string>

class TextureLoader : public ResourceLoader
{
public:
	Object* Load(const std::string& filename);
};

#endif