#include "AmbientLightComponent.h"

#include "Renderer/RenderManager.h"

AmbientLightComponent::AmbientLightComponent () :
	_renderLightObject (new RenderAmbientLightObject ())
{

}

AmbientLightComponent::~AmbientLightComponent ()
{
	delete _renderLightObject;
}

void AmbientLightComponent::Awake ()
{
	_renderLightObject->color = _color;
	_renderLightObject->intensity = _intensity;
	_renderLightObject->isActive = _parent->IsActive ();
}

void AmbientLightComponent::SetActive (bool isActive)
{
	_renderLightObject->isActive = isActive;
}

Color AmbientLightComponent::GetColor () const
{
	return _color;
}

float AmbientLightComponent::GetIntensity () const
{
	return _intensity;
}

void AmbientLightComponent::SetColor (const Color& color)
{
	_renderLightObject->color = color;
}

void AmbientLightComponent::SetIntensity (float intensity)
{
	_renderLightObject->intensity = intensity;
}

void AmbientLightComponent::OnAttachedToScene ()
{
	RenderManager::Instance ()->SetRenderAmbientLightObject (_renderLightObject);
}

void AmbientLightComponent::OnDetachedFromScene ()
{
	RenderManager::Instance ()->SetRenderAmbientLightObject (nullptr);
}
