#include "GameEngine.h"

#include "Systems/Window/Window.h"
#include "Systems/Screen/Screen.h"
#include "Systems/Input/Input.h"
#include "Systems/Time/Time.h"

#include "Managers/ShaderManager.h"
#include "Managers/SceneManager.h"

#include "Skybox/Skybox.h"

#include "Arguments/ArgumentsAnalyzer.h"

#include "Core/Console/Console.h"

#include "Wrappers/OpenGL/GL.h"

// #include "Debug/Debugger.h"

#include "Modules/SDLModule.h"

/*
 * TODO: Change this to Game Engine properties
*/

#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720
#define DEFAULT_WINDOW_TITLE "Lite Engine"

/*
 * TODO (Medium Priority): Refactor initialization responsabilities
*/

void GameEngine::Init ()
{
	SDLModule::Init ();

	Window::Init (DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_TITLE);

	InitOpenGL ();

	// Debugger::Init ();

	Input::Init ();

	Skybox::Init ();

	InitScene ();
}

void GameEngine::Clear ()
{
	ShaderManager::Instance()->Clear();
	SceneManager::Instance()->Clear();

	Window::Close ();

	SDLModule::Quit ();
}

void GameEngine::InitOpenGL ()
{
	// GL::Viewport(0, 0, 10, 10);

	GL::ClearColor( 0.5f, 0.5f, 1.0f, 1.0f );
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

	if (arg == nullptr)
	{
		Console::LogError ("There is no scene to load!");
		exit (0);
	}

	SceneManager::Instance ()->Load (arg->GetArgs () [0]);
}