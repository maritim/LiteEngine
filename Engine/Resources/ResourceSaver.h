#ifndef RESOURCESAVER_H
#define RESOURCESAVER_H

#include <string>

#include "Core/Interfaces/Object.h"

class ResourceSaver
{
public:
	virtual ~ResourceSaver ();

	virtual bool Save (const Object* object, const std::string& filename) = 0;
};

#endif