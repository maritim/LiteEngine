#include "SettingsLoader.h"

Object* SettingsLoader::Load (const std::string& filename)
{
	SimpleIni doc;

	if (!doc.Load (filename)) {
		return nullptr;
	}

	SettingsContainer* settings = new SettingsContainer (doc);

	return settings;
}
