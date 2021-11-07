#ifndef GAMEMODULELOADER_H
#define GAMEMODULELOADER_H

#include <string>

#include "Main/GameModule.h"

class GameModuleLoader
{
public:
	static GameModule* LoadGameModule (const std::string& gameModuleName);
private:
	static std::string GetGameModuleFilename (const std::string& gameModuleName);
};

#endif