#include "GameEngine.h"

#include "Systems/Settings/SettingsManager.h"
#include "Systems/Window/Window.h"
#include "Systems/Screen/Screen.h"
#include "Systems/Input/Input.h"
#include "Systems/Cursor/Cursor.h"
#include "Systems/Time/Time.h"
#include "Systems/Physics/Physics.h"
#include "Systems/GUI/GUI.h"

#include "Managers/SceneManager.h"
#include "Renderer/RenderManager.h"
#include "Renderer/RenderModuleManager.h"

#include "Arguments/ArgumentsAnalyzer.h"

#include "Core/Console/Console.h"

#include "Wrappers/OpenGL/GL.h"

// #include "Debug/Debugger.h"

#include "Modules/SDLModule.h"
#include "Modules/OpenALModule.h"

#include "Renderer/Pipeline.h"

// Change this
#define ENGINE_SETTINGS_PATH "Assets/LiteEditor.ini"

/*
 * TODO (Medium Priority): Refactor initialization responsabilities
*/

void GameEngine::Init ()
{
	InitSettings ();

	Console::Init ();

	SDLModule::Init ();
	OpenALModule::Init ();

	Window::Init ();

	InitOpenGL ();

	// Debugger::Init ();

	Input::Init ();

	Cursor::Init ();

	Physics::Init ();

	RenderManager::Instance ()->Init ();

	GUI::Init ();

	Pipeline::Init ();

	InitScene ();
}

void GameEngine::Clear ()
{
	SceneManager::Instance()->Clear();
	RenderManager::Instance()->Clear();
	RenderModuleManager::Instance ()->Clear ();

	Pipeline::Clear ();

	GUI::Clear ();

	Physics::Quit ();

	Cursor::Clear ();

	Window::Close ();

	OpenALModule::Quit ();
	SDLModule::Quit ();

	Console::Quit ();
}

void GameEngine::InitSettings ()
{
	std::string settingsPath = ENGINE_SETTINGS_PATH;

	Argument* arg = ArgumentsAnalyzer::Instance ()->GetArgument ("configpath");

	if (arg != nullptr) {
		settingsPath = arg->GetArgs () [0];
	}

	SettingsManager::Instance ()->Init (settingsPath);
}

void GameEngine::InitOpenGL ()
{
	// GL::Viewport(0, 0, 10, 10);

	GL::ClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	GL::ClearDepth( 1.0f );
	// GL::Hint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	GL::DepthFunc( GL_LEQUAL );
	GL::Enable( GL_DEPTH_TEST );
	GL::Enable(GL_CULL_FACE);
	GL::Enable(GL_BLEND);

	GL::CullFace (GL_BACK);
	GL::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::string error = (char*) glewGetErrorString(err);
		Console::LogError (error);
	}

	std::string glewvers = (char*) glewGetString(GLEW_VERSION);
	Console::Log ("Status: Using GLEW " + glewvers);

	if (glewIsSupported ("GL_VERSION_4_5")) {
		Console::Log ("Ready for OpenGL 4.5");
	} else {
		Console::LogError ("OpenGL 4.5 not supported");
	}
}

void GameEngine::InitScene ()
{
	Argument* arg = ArgumentsAnalyzer::Instance ()->GetArgument ("startscene");

	if (arg != nullptr) {
		SceneManager::Instance ()->Load (arg->GetArgs () [0]);
	}

	if (arg == nullptr) {
		std::string scenePath = SettingsManager::Instance ()->GetValue<std::string> (
			"Scene", "scene_path", std::string ()
		);

		if (scenePath != std::string ()) {
			SceneManager::Instance ()->Load (scenePath);
		}

		if (scenePath == std::string ()) {
			SceneManager::Instance ()->CreateScene ();
		}
	}
}