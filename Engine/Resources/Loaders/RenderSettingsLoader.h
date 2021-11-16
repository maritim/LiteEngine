#ifndef RENDERSETTINGSLOADER_H
#define RENDERSETTINGSLOADER_H

#include <tinyxml2.h>

#include "Resources/ResourceLoader.h"

#include "Renderer/RenderSettings.h"

class RenderSettingsLoader : public ResourceLoader
{
public:
	Object* Load(const std::string& fileName);
protected:
	void ProcessRenderMode (tinyxml2::XMLElement* xmlElem, RenderSettings* settings);
	void ProcessGeneral (tinyxml2::XMLElement* xmlElem, RenderSettings* settings);
	void ProcessSSAO (tinyxml2::XMLElement* xmlElem, RenderSettings* settings);
	void ProcessSSDO (tinyxml2::XMLElement* xmlElem, RenderSettings* settings);
	void ProcessSSR (tinyxml2::XMLElement* xmlElem, RenderSettings* settings);
	void ProcessTAA (tinyxml2::XMLElement* xmlElem, RenderSettings* settings);
	void ProcessVolLighting (tinyxml2::XMLElement* xmlElem, RenderSettings* settings);
	void ProcessLightShafts (tinyxml2::XMLElement* xmlElem, RenderSettings* settings);
	void ProcessBloom (tinyxml2::XMLElement* xmlElem, RenderSettings* settings);
	void ProcessHDR (tinyxml2::XMLElement* xmlElem, RenderSettings* settings);
	void ProcessLUT (tinyxml2::XMLElement* xmlElem, RenderSettings* settings);
	void ProcessGamma (tinyxml2::XMLElement* xmlElem, RenderSettings* settings);

	void ProcessRSM (tinyxml2::XMLElement* xmlElem, RenderSettings* settings);
	void ProcessTRSM (tinyxml2::XMLElement* xmlElem, RenderSettings* settings);
	void ProcessLPV (tinyxml2::XMLElement* xmlElem, RenderSettings* settings);
	void ProcessVCT (tinyxml2::XMLElement* xmlElem, RenderSettings* settings);
	void ProcessHGI (tinyxml2::XMLElement* xmlElem, RenderSettings* settings);
};

#endif