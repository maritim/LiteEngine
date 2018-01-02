#include "DirectionalLight.h"

#include "LightsManager.h"

#include "Utils/Primitives/Primitive.h"

#include "Lighting/DirectionalLightRenderer.h"

DirectionalLight::DirectionalLight ()
{
	SetVolume (Primitive::Instance ()->Create (Primitive::Type::QUAD));
}

void DirectionalLight::Update ()
{
	
}

void DirectionalLight::SetShadowCasting (bool casting)
{
	ChangeShadowCasting (casting);
}

void DirectionalLight::OnAttachedToScene ()
{
	LightsManager::Instance ()->AddDirectionalLight (this);
}

void DirectionalLight::OnDetachedFromScene ()
{
	LightsManager::Instance ()->RemoveDirectionalLight (this);
}

void DirectionalLight::ChangeShadowCasting (bool casting)
{
	_castShadows = casting;
}
