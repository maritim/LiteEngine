#ifndef SKYBOXLOADER_H
#define SKYBOXLOADER_H

#include "ResourceLoader.h"

#include <string>

#include "Core/Parsers/XML/TinyXml/tinyxml.h"
#include "Skybox/Skybox.h"

class SkyboxLoader : public ResourceLoader
{
public:
	Object* Load (const std::string& filename);
private:
	void ProcessFaces (TiXmlElement* xmlElem, Skybox* skybox, const std::string& filename);
	void ProcessTintColor (TiXmlElement* xmlElem, Skybox* skybox);
	void ProcessExposure (TiXmlElement* xmlElem, Skybox* skybox);
	void ProcessRotation (TiXmlElement* xmlElem, Skybox* skybox);
};

#endif