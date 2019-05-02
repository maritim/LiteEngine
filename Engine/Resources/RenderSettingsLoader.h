#ifndef RENDERSETTINGSLOADER_H
#define RENDERSETTINGSLOADER_H

#include "ResourceLoader.h"

#include "Core/Parsers/XML/TinyXml/tinyxml.h"

#include "Renderer/RenderSettings.h"

class RenderSettingsLoader : public ResourceLoader
{
public:
	Object* Load(const std::string& fileName);
protected:
	void ProcessSSAO (TiXmlElement* xmlElem, RenderSettings* settings);
	void ProcessSSR (TiXmlElement* xmlElem, RenderSettings* settings);
	void ProcessBloom (TiXmlElement* xmlElem, RenderSettings* settings);
	void ProcessHDR (TiXmlElement* xmlElem, RenderSettings* settings);
	void ProcessLUT (TiXmlElement* xmlElem, RenderSettings* settings);
	void ProcessGamma (TiXmlElement* xmlElem, RenderSettings* settings);
};

#endif