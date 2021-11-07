#include "GameModuleLoader.h"

#ifdef _WIN32
	#include <windows.h>
#elif defined(__linux__)
	#include <dlfcn.h>
#endif

GameModule* GameModuleLoader::LoadGameModule (const std::string& gameModuleName)
{
	std::string gameModuleFilename = GetGameModuleFilename (gameModuleName);

#ifdef _WIN32
	HMODULE handle = LoadLibrary(gameModuleFilename.c_str ());
#else
	void* handle = dlopen(gameModuleFilename.c_str (), RTLD_LAZY);
#endif

	GameModule* (*createGameModule)();
	// void (*destroyGameModule)(GameModule*);


#ifdef _WIN32
    createGameModule = (GameModule* (*)())GetProcAddress(handle, "CreateGameModule");
    FARPROC destroyGameModule = GetProcAddress(handle, "DestroyGameModule");
#else
	createGameModule = (GameModule* (*)())dlsym(handle, "CreateGameModule");
	// destroyGameModule = (void (*)(GameModule*))dlsym(handle, "DestroyGameModule");
#endif

	return (GameModule*) createGameModule ();
}

std::string GameModuleLoader::GetGameModuleFilename (const std::string& gameModuleName)
{
	std::string gameModuleFilename;

#ifdef _WIN32
	gameModuleFilename = gameModuleName + ".dll";
#else
	gameModuleFilename = "lib" + gameModuleFilename + ".so";
#endif

	return gameModuleFilename;
}
