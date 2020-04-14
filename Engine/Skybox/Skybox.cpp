#include "Skybox.h"

#include "Utils/Primitives/Primitive.h"

#include "Renderer/RenderSystem.h"
#include "Renderer/RenderManager.h"

Skybox::Skybox () :
    _renderSkyboxObject (new RenderSkyboxObject ())
{
    Resource<Model> cube = Primitive::Instance ()->Create (Primitive::Type::CUBE);

    Resource<ModelView> _skyboxModelView = RenderSystem::LoadModel (cube);

    _renderSkyboxObject->SetModelView (_skyboxModelView);
}

Skybox::~Skybox ()
{
    delete _renderSkyboxObject;
}

void Skybox::SetCubeMap (const Resource<Texture>& cubemap)
{
    Resource<TextureView> textureView = RenderSystem::LoadCubeMap (cubemap);

    _renderSkyboxObject->SetCubeMap (textureView);
}

void Skybox::SetTintColor (Color tintColor)
{
    _renderSkyboxObject->SetTintColor (tintColor);
}

void Skybox::SetBrightness (float brightness)
{
    _renderSkyboxObject->SetBrightness (brightness);
}

void Skybox::SetAngularVelocity (float velocity)
{
    _renderSkyboxObject->SetAngularVelocity (velocity);
}

void Skybox::OnAttachedToScene ()
{
    RenderManager::Instance ()->SetRenderSkyboxObject (_renderSkyboxObject);
}

void Skybox::OnDetachedFromScene ()
{
    RenderManager::Instance ()->SetRenderSkyboxObject (nullptr);
}
