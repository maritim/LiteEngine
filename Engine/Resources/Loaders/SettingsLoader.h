#ifndef SETTINGSLOADER_H
#define SETTINGSLOADER_H

#include "Resources/ResourceLoader.h"

#include "Core/Parsers/INI/SimpleIni/SimpleIni.h"

#include "Core/Settings/SettingsContainer.h"

class SettingsLoader : public ResourceLoader
{
public:
	Object* Load (const std::string& filename);
protected:
	void ProcessSettings (SimpleIni& simpleIni, SettingsContainer* settings);
};

#endif