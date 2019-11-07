#ifndef LIGHTLOADER_H
#define LIGHTLOADER_H

#include "Resources/ResourceLoader.h"

#include <string>

#include "Core/Parsers/XML/TinyXml/tinyxml.h"

#include "Lighting/Light.h"
#include "Lighting/DirectionalLight.h"
#include "Lighting/PointLight.h"
#include "Lighting/SpotLight.h"
#include "Lighting/AmbientLight.h"

class LightLoader : public ResourceLoader
{
public:
	Object* Load (const std::string& filename);
private:
	Light* GetLight (TiXmlElement* xmlElem);

	void ProcessColor (TiXmlElement* xmlElem, Light* light);
	void ProcessIntensity (TiXmlElement* xmlElem, Light* light);
	void ProcessShadows (TiXmlElement* xmlElem, Light* light);

	void ProcessPointLight (TiXmlElement* xmlElem, Light* light);
	void ProcessSpotLight (TiXmlElement* xmlElem, Light* light);

	void ProcessRange (TiXmlElement* xmlElem, PointLight* light);

	void ProcessSpotCutoff (TiXmlElement* xmlElem, SpotLight* light);
	void ProcessSpotOuterCutoff (TiXmlElement* xmlElem, SpotLight* light);

	void ProcessShadowResolution (TiXmlElement* xmlElem, Light::Shadow& shadow);
	void ProcessShadowCascades (TiXmlElement* xmlElem, Light::Shadow& shadow);
	void ProcessShadowBias (TiXmlElement* xmlElem, Light::Shadow& shadow);

	Color GetColor (TiXmlElement* xmlElem);
};

#endif