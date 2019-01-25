#include "SettingsObserverArgs.h"

SettingsObserverArgs::SettingsObserverArgs (const std::string& name) :
	_name (name)
{

}

const std::string& SettingsObserverArgs::GetName () const
{
	return _name;
}