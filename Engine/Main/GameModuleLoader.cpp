#include "GameModuleLoader.h"

#ifdef _WIN32
	#include <windows.h>
#elif defined(__linux__)
	#include <dlfcn.h>
#endif

GameModule* GameModuleLoader::LoadGameModule (const std::string& gameModulePath)
{
#ifdef _WIN32
	HMODULE handle = LoadLibrary(gameModulePath.c_str ());
#else
	void* handle = dlopen(gameModulePath.c_str (), RTLD_LAZY);
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
