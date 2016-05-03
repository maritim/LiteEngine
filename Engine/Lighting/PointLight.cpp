#include "PointLight.h"

#include "LightsManager.h"

#include "Utils/Primitives/Primitive.h"

PointLight::PointLight () :
	_constantAttenuation (1.0),
	_linearAttenuation (0.1),
	_quadraticAttenuation (0.01)
{
	SetVolume (Primitive::Instance ()->Create (Primitive::Type::SPHERE));
}

void PointLight::Update ()
{
	
}

float PointLight::GetConstantAttenuation () const
{
	return _constantAttenuation;
}

float PointLight::GetLinearAttenuation () const
{
	return _linearAttenuation;
}

float PointLight::GetQuadraticAttenuation () const
{
	return _quadraticAttenuation;
}

void PointLight::SetConstantAttenuation (float constantAttenuation)
{
	_constantAttenuation = constantAttenuation;
}

void PointLight::SetLinearAttenuation (float linearAttenuation)
{
	_linearAttenuation = linearAttenuation;
}

void PointLight::SetQuadraticAttenuation (float quadraticAttenuation)
{
	_quadraticAttenuation = quadraticAttenuation;
}

void PointLight::OnAttachedToScene ()
{
	LightsManager::Instance ()->AddPointLight (this);
}

void PointLight::OnDetachedFromScene ()
{
	LightsManager::Instance ()->RemovePointLight (this);
}