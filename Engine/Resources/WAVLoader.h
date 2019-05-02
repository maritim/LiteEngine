#ifndef WAVLOADER_H
#define WAVLOADER_H

#include "ResourceLoader.h"

class WAVLoader : public ResourceLoader
{
public:
	Object* Load(const std::string& filename);
};

#endif