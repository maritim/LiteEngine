#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <map>
#include <queue>
#include <string>

#include "Core/Settings/SettingsContainer.h"
#include "Core/Observer/ObserverI.h"

#include "SettingsObserverArgs.h"

class ENGINE_API SettingsManager : public Singleton<SettingsManager>
{
	friend Singleton<SettingsManager>;

	DECLARE_SINGLETON(SettingsManager)

private:
	SettingsContainer* _settingsContainer;

	std::map<std::string, std::vector<ObserverI<SettingsObserverArgs>*>> _observers;
	std::queue<std::pair<std::string, std::string>> _eventsQueue;

public:
	void Init (const std::string& settingsPath);

	void Update ();

	void SetValue (const std::string& key, const std::string& value);
	template <class T>
	T GetValue (const std::string& key, T defaultValue);

	void Attach (const std::string& key, ObserverI<SettingsObserverArgs>* observer);
	void Detach (const std::string& key, ObserverI<SettingsObserverArgs>* observer);
private:
	SettingsManager ();
	~SettingsManager ();
	SettingsManager (const SettingsManager&);
	SettingsManager& operator= (const SettingsManager&);
};

template <class T>
T SettingsManager::GetValue (const std::string& key, T defaultValue)
{
	return _settingsContainer->GetValue<T> (key, defaultValue);
}

#endif