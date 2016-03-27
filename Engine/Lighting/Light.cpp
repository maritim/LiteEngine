#include "Light.h"

#include "LightsManager.h"

Light::Light () :
	_type (Type::POINT_LIGHT),
	_color (Color::White),
	_specularColor (Color::White),
	_constantAttenuation (1.0),
	_linearAttenuation (0.1),
	_quadraticAttenuation (0.01),
	_spotCutoff (360.0),
	_spotExponent (1.0),
	_spotDirection (1.0, 1.0, 1.0)
{
	_rigidbody->SetGravityUse (0);
}

Light::~Light ()
{

}

Light::Type Light::GetType () const
{
	return _type;
}

Color Light::GetColor () const
{
	return _color;
}

Color Light::GetSpecularColor () const
{
	return _specularColor;
}

float Light::GetConstantAttenuation () const
{
	return _constantAttenuation;
}

float Light::GetLinearAttenuation () const
{
	return _linearAttenuation;
}

float Light::GetQuadraticAttenuation () const
{
	return _quadraticAttenuation;
}

Vector3 Light::GetSpotDirection () const
{
	return _spotDirection;
}

float Light::GetSpotCutoff () const
{
	return _spotCutoff;
}

float Light::GetSpotExponent () const
{
	return _spotExponent;
}

void Light::SetType (Light::Type type)
{
	_type = type;
}

void Light::SetColor (const Color& color)
{
	_color = color;
}

void Light::SetSpecularColor (const Color& specularColor)
{
	_specularColor = specularColor;
}

void Light::SetConstantAttenuation (float constantAttenuation)
{
	_constantAttenuation = constantAttenuation;
}

void Light::SetLinearAttenuation (float linearAttenuation)
{
	_linearAttenuation = linearAttenuation;
}

void Light::SetQuadraticAttenuation (float quadraticAttenuation)
{
	_quadraticAttenuation = quadraticAttenuation;
}

void Light::SetSpotDirection (const Vector3& spotDirection)
{
	_spotDirection = spotDirection;
}

void Light::SetSpotCutoff (float spotCutoff)
{
	_spotCutoff = spotCutoff;
}

void Light::SetSpotExponent (float spotExponent)
{
	_spotExponent = spotExponent;
}

void Light::Update ()
{

}

void Light::OnAttachedToScene ()
{
	LightsManager::Instance ()->AddLight (this);
}

void Light::OnDetachedFromScene ()
{
	LightsManager::Instance ()->RemoveLight (this);
}