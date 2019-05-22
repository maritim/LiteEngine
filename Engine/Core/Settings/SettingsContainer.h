#ifndef SETTINGSCONTAINER_H
#define SETTINGSCONTAINER_H

#include "Core/Interfaces/Object.h"

#include <map>
#include <string>
#include <glm/vec2.hpp>

class ENGINE_API SettingsContainer : public Object
{
private:
	std::map<std::string, std::string> _hash;

public:
	void SetValue (const std::string& key, const std::string& value);

	template <class T>
	T GetValue (const std::string& key, T defaultValue);
};

template <>
std::string ENGINE_API SettingsContainer::GetValue (const std::string& key, std::string defaultValue);

template <>
int ENGINE_API SettingsContainer::GetValue (const std::string& key, int defaultValue);

template <>
float ENGINE_API SettingsContainer::GetValue (const std::string& key, float defaultValue);

template <>
bool ENGINE_API SettingsContainer::GetValue (const std::string& key, bool defaultValue);

template <>
glm::vec2 ENGINE_API SettingsContainer::GetValue (const std::string& key, glm::vec2 defaultValue);

#endif