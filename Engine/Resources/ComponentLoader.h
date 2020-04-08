#ifndef COMPONENTLOADER_H
#define COMPONENTLOADER_H

#include "Core/Interfaces/Object.h"

#include "Core/Parsers/XML/TinyXml/tinyxml.h"

#include "Systems/Components/Component.h"

#include "Core/ObjectsFactory/ObjectsFactory.h"

class ComponentLoader : public Object
{
public:
	virtual Component* Load (TiXmlElement* xmlElem) = 0;
};

#define REGISTER_COMPONENT_LOADER(COMPONENT_LOADER) static RegisterObject<ComponentLoader, COMPONENT_LOADER> dummyLoader (#COMPONENT_LOADER);

#endif