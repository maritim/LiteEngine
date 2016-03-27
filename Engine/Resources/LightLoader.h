#ifndef LIGHTLOADER_H
#define LIGHTLOADER_H

#include "ResourceLoader.h"

#include <string>

#include "Core/Parsers/XML/TinyXml/tinyxml.h"
#include "Lighting/Light.h"

class LightLoader : public ResourceLoader
{
public:
	Object* Load (const std::string& filename);
private:
	void ProcessType (TiXmlElement* xmlElem, Light* light);
	void ProcessAttenuation (TiXmlElement* xmlElem, Light* light);
	void ProcessSpotLight (TiXmlElement* xmlElem, Light* light);
	void ProcessColor (TiXmlElement* xmlElem, Light* light);
	void ProcessSpecularColor (TiXmlElement* xmlElem, Light* light);

	void ProcessSpotCutoff (TiXmlElement* xmlElem, Light* light);
	void ProcessSpotExponent (TiXmlElement* xmlElem, Light* light);
	void ProcessSpotDirection (TiXmlElement* xmlElem, Light* light);

	Color GetColor (TiXmlElement* xmlElem);
};

#endif