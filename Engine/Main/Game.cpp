#include "Game.h"

#include "Systems/Time/Time.h"
#include "Systems/Input/Input.h"
#include "Systems/Window/Window.h"
#include "Systems/Physics/PhysicsSystem.h"
#include "Systems/Components/ComponentManager.h"

#include "Debug/Profiler/Profiler.h"

#include "Renderer/RenderManager.h"

#include "Managers/SceneManager.h"

#define KEY_ESCAPE 27

#define FRAMES_PER_SECOND 1000
#define TICKS_PER_FRAME (1000 / FRAMES_PER_SECOND)
#define MILLISECONDS_PER_FRAME (1.0 / FRAMES_PER_SECOND)

Game::Game ()
{

}

Game::~Game ()
{

}

void Game::Start ()
{
	bool running = true;

	Time::Init ();

	// SDL_GL_SetSwapInterval (1);

	while(running)
	{
		PROFILER_FRAME
		PROFILER_LOGGER("Frame")

		Time::UpdateFrame();
		Input::UpdateState ();

		if (Input::GetQuit () || Input::GetKeyDown (27)) {
			running = false;
			continue;
		}

		if (Input::GetResizeEvent () != glm::vec3 (0.0f)) {
			Window::Resize (Input::GetResizeEvent ());
		}

		UpdateScene ();
		DisplayScene ();

		Window::SwapBuffers ();			

		// if(TICKS_PER_FRAME > Time::GetElapsedTimeMS () - Time::GetTimeMS ()) {
		// 	SDL_Delay(TICKS_PER_FRAME - (Time::GetElapsedTimeMS () - Time::GetTimeMS ()));
		// }
	}
}
	
void Game::UpdateScene() 
{
	PROFILER_LOGGER("Update")

	SceneManager::Instance ()->Current ()->Update ();

	ComponentManager::Instance ()->Update ();
	PhysicsSystem::Instance ().UpdateScene ();
}

void Game::DisplayScene() 
{
	PROFILER_LOGGER("Render")

	RenderManager::Instance ()->RenderScene (SceneManager::Instance ()->Current (), Camera::Main ());
}
