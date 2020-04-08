#ifndef RENDEROBJECTCOMPONENTLOADER_H
#define RENDEROBJECTCOMPONENTLOADER_H

#include "Resources/ComponentLoader.h"

#include "Components/RenderObjectComponent.h"

class RenderObjectComponentLoader : public ComponentLoader
{
public:
	Component* Load (TiXmlElement* xmlElem);
protected:
	void ProcessModel (TiXmlElement* xmlElem, RenderObjectComponent* renderObjectComponent);

	RenderObject* GetRenderObject (const std::string& layer);
};

REGISTER_COMPONENT_LOADER(RenderObjectComponentLoader)

#endif