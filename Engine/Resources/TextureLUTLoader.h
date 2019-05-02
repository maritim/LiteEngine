#ifndef TEXTURELUTLOADER_H
#define TEXTURELUTLOADER_H

#include "ResourceLoader.h"

class TextureLUTLoader : public ResourceLoader
{
public:
	Object* Load(const std::string& filename);
protected:
	void ReorganizePixels (unsigned char* pixels, std::size_t size);
};

#endif