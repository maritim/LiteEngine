#include "SettingsContainer.h"

#include <glm/vec2.hpp>

#include "Utils/Extensions/StringExtend.h"

void SettingsContainer::SetValue (const std::string& key, const std::string& value)
{
	_hash [key] = value;
}

template <>
std::string SettingsContainer::GetValue<std::string> (const std::string& key, std::string defaultValue)
{
	auto it = _hash.find (key);

	if (it == _hash.end ()) {
		return defaultValue;
	}

	return it->second;
}

template <>
int SettingsContainer::GetValue<int> (const std::string& key, int defaultValue)
{
	std::string value = GetValue<std::string> (key, std::to_string (defaultValue));

	return std::stoi (value);
}

template <>
float SettingsContainer::GetValue<float> (const std::string& key, float defaultValue)
{
	std::string value = GetValue<std::string> (key, std::to_string (defaultValue));

	return std::stof (value);
}

template <>
bool SettingsContainer::GetValue<bool> (const std::string& key, bool defaultValue)
{
	std::string value = GetValue<std::string> (key, std::to_string (defaultValue));

	return Extensions::StringExtend::ToBoolExt (value);
}

template <>
glm::vec2 SettingsContainer::GetValue<glm::vec2> (const std::string& key, glm::vec2 defaultValue)
{
	std::string defaultValueS =
		std::to_string (defaultValue.x) + "," + std::to_string (defaultValue.y);

	std::string value = GetValue<std::string> (key, defaultValueS);

	std::vector<std::string> split = Extensions::StringExtend::Split (value, ",");

	if (split.size () != 2) {
		return defaultValue;
	}

	return glm::vec2 (std::stoi (split [0]), std::stoi (split [1]));
}
