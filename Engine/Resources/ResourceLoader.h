#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

#include <string>

#include "Core/Interfaces/Object.h"

class ResourceLoader
{
public:
	virtual ~ResourceLoader ();

	virtual Object* Load(const std::string& fileName) = 0;
};

#endif