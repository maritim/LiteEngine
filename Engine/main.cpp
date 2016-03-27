#include <SDL/SDL.h>
#include <GL/glew.h>

#include <string>
#include <cmath>
#include <unistd.h>

#include "Camera/Camera.h"
#include "Skybox/Skybox.h"
#include "SceneGraph/Scene.h"
#include "SceneNodes/FrameRate.h"
#include "Lighting/Lights.h"

#include "Systems/Input/Input.h"
#include "Systems/Time/Time.h"
#include "Systems/Physics/PhysicsSystem.h"
#include "Systems/Screen/Screen.h"

#include "Renderer/Pipeline.h"
#include "Renderer/RenderManager.h"

#include "Resources/SceneLoader.h"
#include "Resources/Resources.h"

#include "Managers/ShaderManager.h"

#include "Core/Debug/Debug.h"
#include "Core/Console/Console.h"

#include "Core/Math/Vector3.h"

#include "Fonts/BitmapFont.h"

#include "Wrappers/OpenGL/GL.h"

#define KEY_ESCAPE 27
 
using namespace std;
 
/************************************************************************
  Window
 ************************************************************************/
 
typedef struct {
    int width;
	int height;
	char* title;
 
	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;
 
/***************************************************************************
 * Program code
 ***************************************************************************/
 
#define FRAMES_PER_SECOND 1000
#define TICKS_PER_FRAME (1000 / FRAMES_PER_SECOND)
#define MILLISECONDS_PER_FRAME (1.0 / FRAMES_PER_SECOND)

// the variable that keep information about mouse state
bool mouseDown = false;

// the on/off switch of wireframe
bool wireFrameAllowed = false;

// width and height of the window
int h,w;

int mainWindow;

Scene* scene;

glutWindow win;

void update()
{
	// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
	// https://www.youtube.com/watch?v=zZM2uUkEoFw&index=12&list=PLW3Zl3wyJwWOpdhYedlD-yCB7WQoHf-My
	// http://cboard.cprogramming.com/game-programming/135390-how-properly-move-strafe-yaw-pitch-camera-opengl-glut-using-glulookat.html

	float cameraVelocity = 10.0f;
	Vector3 velocity = Vector3::Zero;

	Vector3 Forward = Camera::Main ()->ToVector3();
	Vector3 Right = Forward.Cross (Vector3::Up);
	Forward.Normalize (); Right.Normalize ();

	if (Input::GetKey ('w')) {
		velocity += Forward * cameraVelocity * Time::GetDeltaTime();
	}

	if (Input::GetKey('s')) {
		velocity -= Forward * cameraVelocity * Time::GetDeltaTime();
	}

	if (Input::GetKey('a')) {
		velocity -= Right * cameraVelocity * Time::GetDeltaTime();
	}

	if (Input::GetKey('d')) {
		velocity += Right * cameraVelocity * Time::GetDeltaTime();
	}

	Vector3 camPos = Camera::Main ()->GetPosition () + velocity;
	Camera::Main ()->SetPosition (camPos);

	static bool leftButtonDown = false;
	static int iLastMouseMoveX = 0;
	static int iLastMouseMoveY = 0;

	Vector3 mousePosition = Input::GetMousePosition ();

	if (Input::GetMouseButtonUp (SDL_BUTTON_LEFT)) {
		leftButtonDown = false;
	}
	else if (Input::GetMouseButtonDown (SDL_BUTTON_LEFT)) {
		iLastMouseMoveX = mousePosition.x;
		iLastMouseMoveY = mousePosition.y;

		leftButtonDown = true;
	}

	if (leftButtonDown) {
		int iMoveDistanceX = mousePosition.x - iLastMouseMoveX;
		int iMoveDistanceY = mousePosition.y - iLastMouseMoveY;

		float mouseSensitivity = 0.005f;

		float camYaw = Camera::Main ()->GetYaw () + 
			iMoveDistanceX * mouseSensitivity;
		float camPitch = Camera::Main ()->GetPitch () -
			iMoveDistanceY * mouseSensitivity;
		
		Camera::Main ()->SetYaw (camYaw);
		Camera::Main ()->SetPitch (camPitch);

		iLastMouseMoveX = mousePosition.x;
		iLastMouseMoveY = mousePosition.y;
	}
}

void DisplayScene() 
{
	GL::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Pipeline::SetCameraRotation (Camera::Main ()->ToVector3 ());

	Pipeline::SetCameraPosition (Camera::Main ()->GetPosition ());

	RenderManager::Instance ()->RenderScene (scene);
}	

void UpdateScene() 
{
	scene->Update ();

	PhysicsSystem::Instance ().UpdateScene ();
}
 
void InitEngine () 
{	
	GL::Viewport(0, 0, win.width, win.height);
	GLfloat aspect = (GLfloat) win.width / win.height;
    
	Pipeline::CreatePerspective (win.field_of_view_angle, aspect, win.z_near, win.z_far);

    GL::ClearColor( 0.5f, 0.5f, 1.0f, 1.0f );
    GL::ClearDepth( 1.0f );
    GL::Hint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
 
    GL::DepthFunc( GL_LEQUAL );
    GL::Enable( GL_DEPTH_TEST );
 	GL::Enable(GL_CULL_FACE);
	GL::Enable(GL_BLEND);

 	GL::CullFace (GL_BACK);
	GL::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::string error = (char*) glewGetErrorString(err);
		Console::LogError (error);
	}

	std::string glewvers = (char*) glewGetString(GLEW_VERSION);
	Console::Log ("Status: Using GLEW " + glewvers);

	if (glewIsSupported ("GL_VERSION_4_0")) {
		Console::Log ("Ready for OpenGL 4.0");
	} else {
		Console::LogError ("OpenGL 4.0 not supported");
	}
}

void InitializeEngine (int argc, char **argv)
{
	ShaderManager::AddShader ("DEFAULT", "Assets/Shaders/defaultVertex.glsl", "Assets/Shaders/defaultFragment.glsl");
}

void InitializeScene (int argc, char **argv)
{
	if (argc >= 2) {
		std::string sceneName = argv [1];

		scene = SceneLoader::Instance ().Load (sceneName);

		scene->Init ();

		// TODO: Load this from .scene file
		// Lights

		FrameRate* frameRate = new FrameRate (scene);
		scene->AttachObject (frameRate);

		Light* light = new Light ();
		light->position = Vector3::Right + Vector3::Up + Vector3::Forward;
		light->specularColor = Vector3::One * 0.1f;
		light->type = Light::Type::DIRECTIONAL_LIGHT;

		light = new Light ();
		light->position = Vector3::Left + Vector3::Down + Vector3::Back;
		light->type = Light::Type::POINT_LIGHT;
	} else {
		Console::LogError ("There is no scene to load!");
		exit (0);
	}

	std::string sceneName = argv [1];

	if (scene == nullptr) {
		Console::LogError ("An error occured while tryng to load " + sceneName);
		exit (0);
	}
}

void OnWindowResize (Vector3 dimensions) 
{
	Screen::Instance ()->SetWidth (dimensions.x);
	Screen::Instance ()->SetHeight (dimensions.y);

	SDL_Surface* surface = SDL_GetVideoSurface ();
	SDL_FreeSurface(surface);
	SDL_SetVideoMode((int)dimensions.x,(int)dimensions.y,32,SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);

	glViewport(0,0,dimensions.x, dimensions.y);
	GLfloat aspect = (GLfloat) dimensions.x / dimensions.y;
	
	Pipeline::CreatePerspective (win.field_of_view_angle, aspect, win.z_near, win.z_far);
}

int main(int argc, char **argv) 
{
	Screen::Instance ()->SetWidth (640);
	Screen::Instance ()->SetHeight (480);

	// set window values
	win.width = 640;
	win.height = 480;

	char title[] = "Lite Engine";
	win.title = title;
	win.field_of_view_angle = 45;
	win.z_near = 0.3f;
	win.z_far = 2000.0f;

	SDL_Init(SDL_INIT_EVERYTHING);
	
	SDL_SetVideoMode(640,480,32,SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	bool running=true;
	InitEngine ();

	Skybox::Init ();

	InitializeEngine (argc, argv);

	InitializeScene (argc, argv);
	
	Time::Init();
	
	while(running)
	{
		Time::UpdateFrame();
		Input::UpdateState ();

        if (Input::GetQuit () || Input::GetKeyDown (27)) {
        	running = false;
        	continue;
        }

		if (Input::GetResizeEvent () != Vector3::Zero) {
			OnWindowResize (Input::GetResizeEvent ());
		}

		update ();

		UpdateScene ();
		DisplayScene ();
        
        SDL_GL_SwapBuffers();

		if(TICKS_PER_FRAME > Time::GetElapsedTimeMS () - Time::GetTimeMS ()) {
			SDL_Delay(TICKS_PER_FRAME - (Time::GetElapsedTimeMS () - Time::GetTimeMS ()));
		}
	}

	ShaderManager::Clear ();

	SDL_Quit();

	return 0;
}
