#include "GeneralSettings.h"

/*
 * Singleton part
*/

GeneralSettings::GeneralSettings ()
{

}

GeneralSettings::~GeneralSettings ()
{

}

void GeneralSettings::SetIntValue (const std::string& key, int value)
{
	_intHash [key] = value;
}

int GeneralSettings::GetIntValue (const std::string& key)
{
	auto mapIterrator = _intHash.find (key);

	if (mapIterrator == _intHash.end ()) {
		return 0;
	}

	return mapIterrator->second;
}