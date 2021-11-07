#ifndef SETTINGSSAVER_H
#define SETTINGSSAVER_H

#include "Resources/ResourceSaver.h"

class SettingsSaver : public ResourceSaver
{
public:
	bool Save (const Object* object, const std::string& filename);
};

#endif