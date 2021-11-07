#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <map>
#include <queue>
#include <string>

#include "Core/Settings/SettingsContainer.h"
#include "Core/Observer/ObserverI.h"

#include "SettingsObserverArgs.h"

#include "Resources/Resources.h"

class ENGINE_API SettingsManager : public Singleton<SettingsManager>
{
	friend Singleton<SettingsManager>;

	DECLARE_SINGLETON(SettingsManager)

	// struct OptionChangeEvent
	// {
	// 	std::string section;
	// 	std::string key;
	// 	std::string value;
	// };

private:
	std::string _settingsPath;
	SettingsContainer* _settingsContainer;

	// std::map<std::string, std::vector<ObserverI<SettingsObserverArgs>*>> _observers;
	// std::queue<OptionChangeEvent> _eventsQueue;

public:
	void Init (const std::string& settingsPath);

	// void Update ();

	template <class T>
	void SetValue (const std::string& section, const std::string& key, const T& value);

	template <class T>
	T GetValue (const std::string& section, const std::string& key, const T& defaultValue);

	// void Attach (const std::string& key, ObserverI<SettingsObserverArgs>* observer);
	// void Detach (const std::string& key, ObserverI<SettingsObserverArgs>* observer);
private:
	SettingsManager ();
	~SettingsManager ();
	SettingsManager (const SettingsManager&);
	SettingsManager& operator= (const SettingsManager&);
};

template <class T>
void SettingsManager::SetValue(const std::string& section, const std::string& key, const T& value)
{
	_settingsContainer->SetValue<T> (section, key, value);

	Resources::SaveSettings (_settingsContainer, _settingsPath);
}

template <class T>
T SettingsManager::GetValue (const std::string& section, const std::string& key, const T& defaultValue)
{
	return _settingsContainer->GetValue<T> (section, key, defaultValue);
}

#endif