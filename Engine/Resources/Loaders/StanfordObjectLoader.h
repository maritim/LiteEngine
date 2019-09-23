#ifndef STANFORDOBJECTLOADER_H
#define STANFORDOBJECTLOADER_H

#include "Resources/ResourceLoader.h"

#include <string>

#include "Core/Interfaces/Object.h"

class StanfordObjectLoader : public ResourceLoader
{
public:
	Object* Load(const std::string& filename);
};

#endif