#ifndef PERSISTENTCOMPONENT_H
#define PERSISTENTCOMPONENT_H

#include "Core/Interfaces/Object.h"

#include "Core/Parsers/XML/TinyXml/tinyxml.h"

class ENGINE_API PersistentComponent : public Object
{
public:
	virtual void Load (TiXmlElement* xmlElem) = 0;
	virtual void Save (TiXmlElement* xmlElem) const = 0;
};

#endif