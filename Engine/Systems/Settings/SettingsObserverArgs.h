#ifndef SETTINGSOBSERVERARGS_H
#define SETTINGSOBSERVERARGS_H

#include "Core/Interfaces/Object.h"

#include <string>

class SettingsObserverArgs : public Object
{
protected:
	std::string _name;

public:
	SettingsObserverArgs (const std::string& name);

	virtual const std::string& GetName () const;
};

#endif