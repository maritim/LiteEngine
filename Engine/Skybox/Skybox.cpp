#include "Skybox.h"

#include "Utils/Primitives/Primitive.h"

#include "SkyboxRenderer.h"

#include "Wrappers/OpenGL/GL.h"

Skybox::Skybox () :
    _cubemap (NULL),
    _tintColor (Color::White),
    _brightness (1.0),
    _angularVelocity (0.0)
{
    delete _renderer;
    _renderer = new SkyboxRenderer (this);

    Resource<Model> cube = Primitive::Instance ()->Create (Primitive::Type::CUBE);
    AttachMesh (cube);
}

Skybox::~Skybox ()
{
    unsigned int texture[] = { _cubemap->GetGPUIndex () };
    GL::DeleteTextures(1,texture);
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