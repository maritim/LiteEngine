#ifndef SKYBOXLOADER_H
#define SKYBOXLOADER_H

#include <string>
#include <tinyxml2.h>

#include "Resources/ResourceLoader.h"

#include "Skybox/Skybox.h"

class SkyboxLoader : public ResourceLoader
{
public:
	Object* Load (const std::string& filename);
private:
	void ProcessFaces (tinyxml2::XMLElement* xmlElem, Skybox* skybox, const std::string& filename);
	void ProcessTintColor (tinyxml2::XMLElement* xmlElem, Skybox* skybox);
	void ProcessExposure (tinyxml2::XMLElement* xmlElem, Skybox* skybox);
	void ProcessRotation (tinyxml2::XMLElement* xmlElem, Skybox* skybox);
};

#endif