#include "AmbientLight.h"

#include "Renderer/RenderManager.h"

AmbientLight::AmbientLight () :
	_renderLightObject (new RenderAmbientLightObject ())
{

}

AmbientLight::~AmbientLight ()
{
	delete _renderLightObject;
}

void AmbientLight::SetActive (bool isActive)
{
	SceneObject::SetActive (isActive);

	_renderLightObject->isActive = isActive;
}

void AmbientLight::SetColor (const Color& color)
{
	Light::SetColor (color);

	_renderLightObject->color = color;
}

void AmbientLight::SetIntensity (float intensity)
{
	Light::SetIntensity (intensity);

	_renderLightObject->intensity = intensity;
}

void AmbientLight::Update ()
{
	
}

void AmbientLight::OnAttachedToScene ()
{
	RenderManager::Instance ()->SetRenderAmbientLightObject (_renderLightObject);
}

void AmbientLight::OnDetachedFromScene ()
{
	RenderManager::Instance ()->SetRenderAmbientLightObject (nullptr);
}
