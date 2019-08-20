#include "PointLight.h"

#include <glm/vec3.hpp>
#include <cmath>

#include "Utils/Primitives/Primitive.h"

#include "Renderer/RenderPointLightObject.h"

#include "Renderer/RenderSystem.h"
#include "Renderer/RenderManager.h"

PointLight::PointLight () :
	_attenuation (0, 0, 1)
{
	_renderLightObject = new RenderPointLightObject ();

	Resource<Model> model = Primitive::Instance ()->Create (Primitive::Type::SPHERE);
	Resource<ModelView> modelView = RenderSystem::LoadModel (model);

	_renderLightObject->SetTransform (_transform);
	_renderLightObject->SetModelView (modelView);
}

void PointLight::SetActive (bool isActive)
{
	SceneObject::SetActive (isActive);

	_renderLightObject->SetActive (isActive);
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

void PointLight::SetAttenuation (const glm::vec3& attenuation)
{
	_attenuation = attenuation;

	auto renderLightObject = (RenderPointLightObject*) _renderLightObject;
	renderLightObject->SetLightAttenuation (_attenuation);

	UpdateScale ();
}

void PointLight::OnAttachedToScene ()
{
	auto renderLightObject = (RenderPointLightObject*) _renderLightObject;
	RenderManager::Instance ()->AttachRenderPointLightObject (renderLightObject);
}

void PointLight::OnDetachedFromScene ()
{
	auto renderLightObject = (RenderPointLightObject*) _renderLightObject;
	RenderManager::Instance ()->DetachRenderPointLightObject (renderLightObject);
}

void PointLight::UpdateScale ()
{
	/*
	 * TODO: Move this somewhere else
	*/

	float minIntensity = 5.0f;

	/*
	 * Compute light intensity distance
	*/

	glm::vec3 color = _color.ToVector3 ();
	float maxLightChannel = std::fmax (std::fmax (color.x, color.y), color.z);

	float dist = (-_attenuation.y + std::sqrt (_attenuation.y * _attenuation.y -
		4 * _attenuation.z * (_attenuation.x - (256.0f / minIntensity) * maxLightChannel))) /
		(2 * _attenuation.z);

	/*
	 * Set light volume scale based on light distance
	*/

	_transform->SetScale (glm::vec3 (dist));
}