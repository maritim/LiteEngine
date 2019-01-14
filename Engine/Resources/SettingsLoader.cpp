#include "SettingsLoader.h"

Object* SettingsLoader::Load (const std::string& filename)
{
	SimpleIni doc;

	if (!doc.Load (filename)) {
		return nullptr;
	}

	SettingsContainer* settings = new SettingsContainer ();

	ProcessSettings (doc, settings);

	return settings;
}

void SettingsLoader::ProcessSettings (SimpleIni& simpleIni, SettingsContainer* settings)
{
	for (SimpleIni::SectionIterator itSection = simpleIni.beginSection(); itSection != simpleIni.endSection(); ++itSection) {
		for (SimpleIni::KeyIterator itKey = simpleIni.beginKey(*itSection); itKey != simpleIni.endKey(*itSection); ++itKey) {

			settings->SetValue (*itKey, simpleIni.GetValue<std::string> (*itSection, *itKey, ""));
		}
	}
}
