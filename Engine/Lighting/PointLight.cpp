#include "PointLight.h"

#include <glm/vec3.hpp>
#include <cmath>

#include "Utils/Primitives/Primitive.h"

#include "Renderer/RenderPointLightObject.h"

#include "Renderer/RenderSystem.h"
#include "Renderer/RenderManager.h"

PointLight::PointLight () :
	_range (0.0f)
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
		UpdateTransform ();
	}
}

float PointLight::GetRange () const
{
	return _range;
}

void PointLight::SetRange (float range)
{
	_range = range;

	auto renderLightObject = (RenderPointLightObject*) _renderLightObject;
	renderLightObject->SetLightRange (_range);

	UpdateTransform ();
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

void PointLight::UpdateTransform ()
{
	/*
	 * Set light volume scale based on light distance
	*/

	_transform->SetScale (glm::vec3 (_range));
}