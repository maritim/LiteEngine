#ifndef WAVLOADER_H
#define WAVLOADER_H

#include "Resources/ResourceLoader.h"

class WAVLoader : public ResourceLoader
{
public:
	Object* Load(const std::string& filename);
};

#endif