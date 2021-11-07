#include "SettingsContainer.h"

#include "Utils/Extensions/StringExtend.h"

SettingsContainer::SettingsContainer (const SimpleIni& settings) :
	_settings (settings)
{

}

SimpleIni& SettingsContainer::GetSettings ()
{
	return _settings;
}

template <>
void SettingsContainer::SetValue<bool> (const std::string& section,
	const std::string& key, const bool& value)
{
	std::string valueS = value == true ? "true" : "false";

	_settings.SetValue (section, key, valueS);
}

template <>
void SettingsContainer::SetValue<glm::vec2> (const std::string& section,
	const std::string& key, const glm::vec2& value)
{
	std::string valueS =
		std::to_string (value.x) + "," + std::to_string (value.y);

	_settings.SetValue (section, key, valueS);
}

template <>
void SettingsContainer::SetValue<glm::vec3> (const std::string& section,
	const std::string& key, const glm::vec3& value)
{
	std::string valueS = std::to_string (value.x) + "," +
		std::to_string (value.y) + "," + std::to_string (value.z);

	_settings.SetValue (section, key, valueS);
}

template <>
bool SettingsContainer::GetValue<bool> (const std::string& section,
	const std::string& key, const bool& defaultValue)
{
	std::string value = _settings.GetValue (section, key, std::to_string (defaultValue));

	return Extensions::StringExtend::ToBoolExt (value);
}

template <>
glm::vec2 SettingsContainer::GetValue<glm::vec2> (const std::string& section,
	const std::string& key, const glm::vec2& defaultValue)
{
	std::string defaultValueS =
		std::to_string (defaultValue.x) + "," + std::to_string (defaultValue.y);

	std::string value = _settings.GetValue (section, key, defaultValueS);

	std::vector<std::string> split = Extensions::StringExtend::Split (value, ",");

	if (split.size () != 2) {
		return defaultValue;
	}

	return glm::vec2 (std::stof (split [0]), std::stof (split [1]));
}

template <>
glm::vec3 SettingsContainer::GetValue<glm::vec3> (const std::string& section,
	const std::string& key, const glm::vec3& defaultValue)
{
	std::string defaultValueS = std::to_string (defaultValue.x) + "," +
		std::to_string (defaultValue.y) + "," + std::to_string (defaultValue.z);

	std::string value = _settings.GetValue (section, key, defaultValueS);

	std::vector<std::string> split = Extensions::StringExtend::Split (value, ",");

	if (split.size () != 3) {
		return defaultValue;
	}

	return glm::vec3 (std::stof (split [0]), std::stof (split [1]), std::stof (split [2]));
}


