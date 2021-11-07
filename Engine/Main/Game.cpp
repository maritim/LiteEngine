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

#include "Core/Console/Console.h"

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
	bool running = true;

	Time::Init ();
	InitGameModule ();

	while(running)
	{
		PROFILER_FRAME

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

		// if(TICKS_PER_FRAME > Time::GetElapsedTimeMS () - Time::GetTimeMS ()) {
		// 	SDL_Delay(TICKS_PER_FRAME - (Time::GetElapsedTimeMS () - Time::GetTimeMS ()));
		// }

		Window::SwapBuffers ();
	}
}

void Game::InitGameModule ()
{
	/*
	 * Get game module name
	*/

	std::string gameModuleName = GetGameModuleName ();

	/*
	 * When there is no game module given, stop execution
	*/

	if (gameModuleName == std::string ()) {
		Console::LogError ("There is no game module to load!");
		exit (0);
	}

	/*
	 * Load game module
	*/

	_gameModule = GameModuleLoader::LoadGameModule (gameModuleName);

	/*
	 * Initialize game module
	*/

	_gameModule->Init ();
}

void Game::UpdateScene() 
{
	PROFILER_LOGGER("Update")

	SceneManager::Instance ()->Update ();

	SceneManager::Instance ()->Current ()->Update ();

	_gameModule->UpdateScene ();

	ComponentManager::Instance ()->Update ();
	PhysicsManager::Instance ()->Update ();
	//SettingsManager::Instance ()->Update ();
}

void Game::DisplayScene() 
{
	_gameModule->RenderScene ();

	Camera* camera = CameraManager::Instance ()->GetActive ();

	//TODO: Change this
	RenderSettings* settings = RenderSettingsManager::Instance ()->GetActive ();
	settings->resolution.width = Window::GetWidth ();
	settings->resolution.height = Window::GetHeight ();
	settings->viewport.x = 0;
	settings->viewport.y = 0;
	settings->viewport.width = Window::GetWidth ();
	settings->viewport.height = Window::GetHeight ();

	RenderManager::Instance ()->Render (camera, *settings);
}

std::string Game::GetGameModuleName ()
{
	std::string gameModuleName;

	/*
	 * Retrieve game module name
	*/

	Argument* arg = ArgumentsAnalyzer::Instance ()->GetArgument ("gamemodule");

	if (arg != nullptr) {
		gameModuleName = arg->GetArgs () [0];
	}

	/*
	 * Retrieve game module name
	*/

	if (arg == nullptr) {
		gameModuleName = SettingsManager::Instance ()->GetValue<std::string> (
			"GameModule", "name", std::string ()
		);
	}

	return gameModuleName;
}
