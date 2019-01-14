#ifndef SETTINGSCONTAINER_H
#define SETTINGSCONTAINER_H

#include "Core/Interfaces/Object.h"

#include <map>
#include <string>

class SettingsContainer : public Object
{
private:
	std::map<std::string, std::string> _hash;

public:
	void SetValue (const std::string& key, const std::string& value);

	template <class T>
	T GetValue (const std::string& key, T defaultValue);
};

#endif