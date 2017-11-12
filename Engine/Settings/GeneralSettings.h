#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include "Core/Singleton/Singleton.h"

#include <map>
#include <string>

class GeneralSettings : public Singleton<GeneralSettings>
{
	friend Singleton<GeneralSettings>;

private:
	std::map<std::string, int> _intHash;

public:
	void SetIntValue (const std::string& key, int value);
	int GetIntValue (const std::string& key);
private:
	GeneralSettings ();
	~GeneralSettings ();
	GeneralSettings (const GeneralSettings&);
	GeneralSettings& operator= (const GeneralSettings&);
};

#endif