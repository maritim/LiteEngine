#ifndef SETTINGSCONTAINER_H
#define SETTINGSCONTAINER_H

#include "Core/Interfaces/Object.h"

#include <map>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <SimpleIni/SimpleIni.h>

class ENGINE_API SettingsContainer : public Object
{
private:
	SimpleIni _settings;

public:
	SettingsContainer (const SimpleIni& settings);

	template <class T>
	void SetValue(const std::string& section, const std::string& key, const T& value);

	template <class T>
	T GetValue (const std::string& section, const std::string& key, const T& defaultValue);

	SimpleIni& GetSettings ();
};

template <class T>
void SettingsContainer::SetValue (const std::string& section, const std::string& key, const T& value)
{
	_settings.SetValue<T> (section, key, value);
}

template <class T>
T SettingsContainer::GetValue(const std::string& section, const std::string& key, const T& defaultValue)
{
	return _settings.GetValue<T> (section, key, defaultValue);
}

template <>
void ENGINE_API SettingsContainer::SetValue (const std::string& section, const std::string& key, const bool& value);

template <>
void ENGINE_API SettingsContainer::SetValue (const std::string& section, const std::string& key, const glm::vec2& value);

template <>
void ENGINE_API SettingsContainer::SetValue (const std::string& section, const std::string& key, const glm::vec3& value);

template <>
bool ENGINE_API SettingsContainer::GetValue (const std::string& section, const std::string& key, const bool& defaultValue);

template <>
glm::vec2 ENGINE_API SettingsContainer::GetValue (const std::string& section, const std::string& key, const glm::vec2& defaultValue);

template <>
glm::vec3 ENGINE_API SettingsContainer::GetValue (const std::string& section, const std::string& key, const glm::vec3& defaultValue);

#endif