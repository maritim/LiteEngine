#include "Skybox.h"

#include <GL/glew.h>
#include <string>
#include <vector>

#include "Managers/ShaderManager.h"
#include "SceneGraph/Transform.h"

#include "Resources/Resources.h"

#include "SkyboxRenderer.h"

#include "Mesh/Model.h"

#include "Utils/Primitives/Primitive.h"

Skybox* Skybox::_currentSkybox (NULL);

Skybox::Skybox () :
    _cubemap (NULL),
    _tintColor (Color::White),
    _brightness (1.0),
    _angularVelocity (0.0)
{
    delete _renderer;
    _renderer = new SkyboxRenderer (_cubemap, &_tintColor, &_brightness, "Skybox");
}

Skybox::~Skybox ()
{
    unsigned int texture[] = { _cubemap->GetGPUIndex () };
    glDeleteTextures(1,texture);

    delete _renderer;
}

void Skybox::Init ()
{
    const std::string defaultShader[] = {"Skybox", 
        "Assets/Shaders/skyboxVertex.glsl", 
        "Assets/Shaders/skyboxFragment.glsl"
    } ;

    ShaderManager::AddShader (defaultShader [0], defaultShader [1], defaultShader [2]);
}

void Skybox::Set (Skybox* skybox)
{
    delete _currentSkybox;

    _currentSkybox = skybox;
}

void Skybox::Render ()
{
	if (_currentSkybox == NULL) {
        return ;
    }

   _currentSkybox->GetRenderer ()->Draw ();
}

void Skybox::Clear ()
{
    if (!_currentSkybox) {
        return ;
    }

    delete _currentSkybox;
}

CubeMap* Skybox::GetCurrentCubeMap ()
{
    return _currentSkybox->GetCubeMap ();
}

void Skybox::SetCubeMap (CubeMap* cubemap)
{
    _cubemap = cubemap;
}

CubeMap* Skybox::GetCubeMap () const
{
    return _cubemap;
}

void Skybox::SetTintColor (Color tintColor)
{
    _tintColor = tintColor;
}

Color Skybox::GetTintColor () const
{
    return _tintColor;
}

void Skybox::SetBrightness (float brightness)
{
    _brightness = brightness;
}

float Skybox::GetBrightness () const
{
    return _brightness;
}

void Skybox::SetAngularVelocity (float velocity)
{
    _angularVelocity = velocity;
}

float Skybox::GetAngularVelocity () const
{
    return _angularVelocity;
}