#include "SettingsSaver.h"

#include "Core/Settings/SettingsContainer.h"

#include "Core/Console/Console.h"

bool SettingsSaver::Save (const Object* object, const std::string& filename)
{
	const SettingsContainer* settingsContainer = dynamic_cast<const SettingsContainer*> (object);

	if (settingsContainer == nullptr) {
		Console::LogError ("Could not save \"" + filename + "\" settings!");
		return false;
	}

	((SettingsContainer*) settingsContainer)->GetSettings ().SaveAs (filename);

	return true;
}
