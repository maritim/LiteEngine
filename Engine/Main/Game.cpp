#include "Game.h"

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

#include "GameModuleLoader.h"

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

	_gameModule = GameModuleLoader::LoadGameModule (gameModulePath);
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

	Camera* camera = CameraManager::Instance ()->GetActive ();

	//TODO: Change this
	RenderSettings* settings = RenderSettingsManager::Instance ()->GetActive ();
	settings->framebuffer.width = Window::GetWidth ();
	settings->framebuffer.height = Window::GetHeight ();
	settings->viewport.x = 0;
	settings->viewport.y = 0;
	settings->viewport.width = Window::GetWidth ();
	settings->viewport.height = Window::GetHeight ();

	RenderManager::Instance ()->Render (camera, *settings);
}
