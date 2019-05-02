#include "Game.h"

#include <dlfcn.h>

#include "Systems/Time/Time.h"
#include "Systems/Input/Input.h"
#include "Systems/Window/Window.h"
#include "Systems/Physics/PhysicsManager.h"
#include "Systems/GUI/GUI.h"
#include "Systems/Components/ComponentManager.h"
#include "Systems/Settings/SettingsManager.h"

#include "Debug/Profiler/Profiler.h"

#include "Arguments/ArgumentsAnalyzer.h"

#include "Renderer/RenderManager.h"

#include "Managers/SceneManager.h"
#include "Managers/CameraManager.h"
#include "Managers/RenderSettingsManager.h"

#include "GameModule.h"

#define FRAMES_PER_SECOND 60
#define TICKS_PER_FRAME (1000 / FRAMES_PER_SECOND)
#define MILLISECONDS_PER_FRAME (1.0 / FRAMES_PER_SECOND)

Game::Game () :
	_gameModule (nullptr)
{

}

Game::~Game ()
{
	delete _gameModule;
}

void Game::Start ()
{
	LoadGameModule ();

	_gameModule->Init ();

	bool running = true;

	Time::Init ();

	while(running)
	{
		PROFILER_FRAME
		PROFILER_LOGGER("Frame")

		Time::UpdateFrame();
		Input::UpdateState ();
		GUI::Update ();

		_gameModule->UpdateFrame ();

		/*
		 * Remove Exit on Escape Free from here
		*/

		if (Input::GetQuit ()) {
			running = false;
			continue;
		}

		if (Input::GetResizeEvent () != glm::ivec2 (0)) {
			Window::Resize (Input::GetResizeEvent ());
		}

		UpdateScene ();
		DisplayScene ();

		Window::SwapBuffers ();			

		if(TICKS_PER_FRAME > Time::GetElapsedTimeMS () - Time::GetTimeMS ()) {
			SDL_Delay(TICKS_PER_FRAME - (Time::GetElapsedTimeMS () - Time::GetTimeMS ()));
		}
	}
}

void Game::LoadGameModule ()
{
	Argument* arg = ArgumentsAnalyzer::Instance ()->GetArgument ("gamemodule");

	if (arg == nullptr) {
		Console::LogError ("There is no game module to load!");
		exit (0);
	}

	std::string gameModulePath = arg->GetArgs () [0];

#ifdef _WIN32
	HMODULE handle = LoadLibrary(gameModulePath.c_str ());
#else
	void* handle = dlopen(gameModulePath.c_str (), RTLD_LAZY);
#endif

	GameModule* (*createGameModule)();
	// void (*destroyGameModule)(GameModule*);


#ifdef _WIN32
    FARPROC createGameModule = (GameModule* (*)())GetProcAddress(handle, "CreateGameModule");
    FARPROC destroyGameModule = GetProcAddress(handle, "DestroyGameModule");
#else
	createGameModule = (GameModule* (*)())dlsym(handle, "CreateGameModule");
	// destroyGameModule = (void (*)(GameModule*))dlsym(handle, "DestroyGameModule");
#endif


	_gameModule = (GameModule*) createGameModule ();
}

void Game::UpdateScene() 
{
	PROFILER_LOGGER("Update")

	SceneManager::Instance ()->Update ();
	CameraManager::Instance ()->Update ();

	SceneManager::Instance ()->Current ()->Update ();

	_gameModule->UpdateScene ();

	ComponentManager::Instance ()->Update ();
	PhysicsManager::Instance ()->Update ();
	SettingsManager::Instance ()->Update ();
}

void Game::DisplayScene() 
{
	PROFILER_LOGGER("Render")

	_gameModule->RenderScene ();

	Scene* scene = SceneManager::Instance ()->Current ();
	Camera* camera = CameraManager::Instance ()->GetActive ();

	//TODO: Change this
	RenderSettings* settings = RenderSettingsManager::Instance ()->GetActive ();
	settings->framebuffer.width = Window::GetWidth ();
	settings->framebuffer.height = Window::GetHeight ();
	settings->viewport.x = 0;
	settings->viewport.y = 0;
	settings->viewport.width = Window::GetWidth ();
	settings->viewport.height = Window::GetHeight ();

	RenderManager::Instance ()->RenderScene (scene, camera, *settings);
}
