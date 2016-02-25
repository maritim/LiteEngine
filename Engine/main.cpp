/*

g++ -std=c++11 Licenta.cpp Polygon.cpp Material.cpp Vector3.cpp Graph.cpp Model.cpp Hashmap.cpp CatmullClark.cpp Camera.cpp Resources.cpp Triangulation.cpp Heap.cpp Simplification.cpp Matrix.cpp Array.cpp CostPair.cpp -lGL -lGLU -lglut `sdl-config --libs` -o Licenta && clear && ./Licenta Examples/pig.obj

g++ -g -std=c++11 *.cpp -lGL -lGLU -lGLEW -lglut `sdl-config --libs` -lSDL_image -o Licenta && clear && ./Licenta Examples/cube.obj
*/

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <string.h>
#include <string>
#include <cmath>
#include <sys/time.h>
#include <unistd.h>

#include "Camera/Camera.h"
#include "Skybox/Skybox.h"
#include "SceneGraph/Scene.h"
#include "Lighting/Lights.h"

#include "Systems/Input/Input.h"
#include "Systems/Time/Time.h"
#include "Systems/Physics/PhysicsSystem.h"

#include "Renderer/Pipeline.h"
#include "Renderer/RenderManager.h"

#include "Resources/SceneLoader.h"

#include "Managers/ShaderManager.h"

#include "Core/Debug/Debug.h"
#include "Core/Console/Console.h"

#include "Core/Math/Vector3.h"

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
#define TIME_PER_FRAME (1000 / FRAMES_PER_SECOND)
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

void ceata()
{
	GLfloat fogColor[4]={0.7f, 0.7f, 0.7f, 0.7f};
	glFogf(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 0);
	glFogf(GL_FOG_END, 130);
	glFogfv(GL_FOG_COLOR, fogColor);
	glEnable(GL_FOG);
}

void clean ();

void update()
{
	// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
	// https://www.youtube.com/watch?v=zZM2uUkEoFw&index=12&list=PLW3Zl3wyJwWOpdhYedlD-yCB7WQoHf-My
	// http://cboard.cprogramming.com/game-programming/135390-how-properly-move-strafe-yaw-pitch-camera-opengl-glut-using-glulookat.html

	float cameraVelocity = 10.0f;
	Vector3 velocity;

	Vector3 Forward = Camera::Main ()->ToVector3();
	Forward.Normalize ();
	Vector3 ForwardYZero = Forward;
	ForwardYZero.Normalize ();
	Vector3 Right = ForwardYZero.Cross (Vector3::Up);
	Right.Normalize ();

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, win.width, win.height);
	GLfloat aspect = (GLfloat) win.width / win.height;
    
	Pipeline::CreatePerspective (win.field_of_view_angle, aspect, win.z_near, win.z_far);

    glShadeModel( GL_SMOOTH );
    glClearColor( 0.5f, 0.5f, 1.0f, 1.0f );
    glClearDepth( 1.0f );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
 
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );
 	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

 	glCullFace (GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::string error = (char*) glewGetErrorString(err);
		Console::LogError (error);
	}

	std::string glewvers = (char*) glewGetString(GLEW_VERSION);
	Console::Log ("Status: Using GLEW " + glewvers);

	if (glewIsSupported ("GL_VERSION_3_3")) {
		Console::Log ("Ready for OpenGL 3.3");
	} else {
		Console::LogError ("OpenGL 3.3 not supported");
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
	SDL_Surface* surface = SDL_GetVideoSurface ();
	SDL_FreeSurface(surface);
	SDL_SetVideoMode((int)dimensions.x,(int)dimensions.y,32,SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);

	glViewport(0,0,dimensions.x, dimensions.y);
	GLfloat aspect = (GLfloat) dimensions.x / dimensions.y;
	
	Pipeline::CreatePerspective (win.field_of_view_angle, aspect, win.z_near, win.z_far);
}

int main(int argc, char **argv) 
{
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
        
		Time::UpdateFrame();

        if(MILLISECONDS_PER_FRAME > Time::GetDeltaTime()) {
            SDL_Delay((MILLISECONDS_PER_FRAME - Time::GetDeltaTime()) * 1000);
        }
	}

	ShaderManager::Clear ();

	SDL_Quit();

	return 0;
}
