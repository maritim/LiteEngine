#ifndef SETTINGSLOADER_H
#define SETTINGSLOADER_H

#include "Resources/ResourceLoader.h"

#include <SimpleIni/SimpleIni.h>

#include "Core/Settings/SettingsContainer.h"

class SettingsLoader : public ResourceLoader
{
public:
	Object* Load (const std::string& filename);
};

#endif