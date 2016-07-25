#include "SpotLight.h"

#include "LightsManager.h"

#include "Utils/Primitives/Primitive.h"

SpotLight::SpotLight () :
	_spotCutoff (360.0),
	_spotExponent (1.0),
	_spotDirection (1.0, 1.0, 1.0)
{
	SetVolume (Primitive::Instance ()->Create (Primitive::Type::CONE));
}

void SpotLight::Update ()
{
	
}

glm::vec3 SpotLight::GetSpotDirection () const
{
	return _spotDirection;
}

float SpotLight::GetSpotCutoff () const
{
	return _spotCutoff;
}

float SpotLight::GetSpotExponent () const
{
	return _spotExponent;
}

void SpotLight::SetSpotDirection (const glm::vec3& spotDirection)
{
	_spotDirection = spotDirection;
}

void SpotLight::SetSpotCutoff (float spotCutoff)
{
	_spotCutoff = spotCutoff;
}

void SpotLight::SetSpotExponent (float spotExponent)
{
	_spotExponent = spotExponent;
}

void SpotLight::OnAttachedToScene ()
{
	LightsManager::Instance ()->AddSpotLight (this);
}

void SpotLight::OnDetachedFromScene ()
{
	LightsManager::Instance ()->RemoveSpotLight (this);
}