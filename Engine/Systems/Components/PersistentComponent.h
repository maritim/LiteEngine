#ifndef PERSISTENTCOMPONENT_H
#define PERSISTENTCOMPONENT_H

#include <tinyxml2.h>

#include "Core/Interfaces/Object.h"

class ENGINE_API PersistentComponent : public Object
{
public:
	virtual void Load (tinyxml2::XMLElement* xmlElem) = 0;
	virtual void Save (tinyxml2::XMLElement* xmlElem) const = 0;
};

#endif