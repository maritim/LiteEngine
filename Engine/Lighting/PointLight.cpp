#include "PointLight.h"

#include <cmath>

#include "LightsManager.h"

#include "Utils/Primitives/Primitive.h"

#include "PointLightRenderer.h"

#include "Core/Math/glm/vec3.hpp"

PointLight::PointLight () :
	_constantAttenuation (1.0),
	_linearAttenuation (0.1),
	_quadraticAttenuation (0.01)
{
	delete _lightRenderer;
	_lightRenderer = new PointLightRenderer (this);

	SetVolume (Primitive::Instance ()->Create (Primitive::Type::SPHERE));
}

void PointLight::Update ()
{
	/*
	 * TODO: Remove this after fix transforms.
	*/

	if (_transform->GetScale ().x < 2.0f) {
		UpdateScale ();
	}
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

	UpdateScale ();
}

void PointLight::SetLinearAttenuation (float linearAttenuation)
{
	_linearAttenuation = linearAttenuation;

	UpdateScale ();
}

void PointLight::SetQuadraticAttenuation (float quadraticAttenuation)
{
	_quadraticAttenuation = quadraticAttenuation;

	UpdateScale ();
}

void PointLight::OnAttachedToScene ()
{
	LightsManager::Instance ()->AddPointLight (this);
}

void PointLight::OnDetachedFromScene ()
{
	LightsManager::Instance ()->RemovePointLight (this);
}

void PointLight::UpdateScale ()
{
	glm::vec3 color = _color.ToVector3 ();
	float MaxChannel = fmax(fmax(color.x, color.y), color.z);

	float intensity = 0.01f;

	float ret = (-_linearAttenuation + sqrtf(_linearAttenuation * _linearAttenuation -
		4 * _quadraticAttenuation * (_constantAttenuation - 256 * MaxChannel * intensity))) / 
		(2 * _quadraticAttenuation);

	_transform->SetScale (glm::vec3 (ret / 2, ret / 2, ret / 2));
}