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

void DirectionalLight::OnAttachedToScene ()
{
	LightsManager::Instance ()->AddDirectionalLight (this);
}

void DirectionalLight::OnDetachedFromScene ()
{
	LightsManager::Instance ()->RemoveDirectionalLight (this);
}
