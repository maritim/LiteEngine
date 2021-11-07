#include "SettingsManager.h"

#include <algorithm>

#include "Resources/Resources.h"

SettingsManager::SettingsManager () :
	_settingsContainer (nullptr)
{

}

SettingsManager::~SettingsManager ()
{
	delete _settingsContainer;
}

SPECIALIZE_SINGLETON(SettingsManager)

void SettingsManager::Init (const std::string& settingsPath)
{
	_settingsPath = settingsPath;

	_settingsContainer = Resources::LoadSettings (_settingsPath);
}

// void SettingsManager::SetValue (const std::string& section,
// 	const std::string& key, const std::string& value)
// {
// 	_eventsQueue.push (OptionChangeEvent { section, key, value });
// }

// void SettingsManager::Update ()
// {
// 	while (_eventsQueue.empty () == false) {
// 		auto event = _eventsQueue.front ();

// 		_settingsContainer->SetValue (event.section, event.key, event.value);

// 		_settingsContainer->SetValue<T> (section, key, value);

// 		Resources::SaveSettings (_settingsContainer, _settingsPath);

// 		for (auto observer : _observers [event.key]) {
// 			observer->Notify (this, SettingsObserverArgs (event.key));
// 		}

// 		_eventsQueue.pop ();
// 	}
// }

// void SettingsManager::Attach (const std::string& key, ObserverI<SettingsObserverArgs>* observer)
// {
// 	auto itKey = _observers.find (key);

// 	if (itKey == _observers.end ()) {
// 		auto res = _observers.insert (std::pair<std::string, std::vector<ObserverI<SettingsObserverArgs>*>> (key, std::vector<ObserverI<SettingsObserverArgs>*> ()));
// 		itKey = res.first;
// 	}

// 	itKey->second.push_back (observer);
// }

// void SettingsManager::Detach (const std::string& key, ObserverI<SettingsObserverArgs>* observer)
// {
// 	auto itKey = _observers.find (key);

// 	if (itKey == _observers.end ()) {
// 		return;
// 	}

// 	auto itObserver = std::find (itKey->second.begin (), itKey->second.end (), observer);

// 	if (itObserver == itKey->second.end ()) {
// 		return;
// 	}

// 	std::iter_swap (itObserver, itKey->second.rbegin ());

// 	itKey->second.pop_back ();
// }
