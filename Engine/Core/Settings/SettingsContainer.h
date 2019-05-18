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

	template <>
	std::string GetValue (const std::string& key, std::string defaultValue);

	template <>
	int GetValue (const std::string& key, int defaultValue);

	template <>
	float GetValue (const std::string& key, float defaultValue);

	template <>
	bool GetValue (const std::string& key, bool defaultValue);

	template <>
	glm::vec2 GetValue (const std::string& key, glm::vec2 defaultValue);
};

#endif