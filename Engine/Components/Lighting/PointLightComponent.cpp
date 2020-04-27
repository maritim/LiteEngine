#include "PointLightComponent.h"

#include "Utils/Primitives/Primitive.h"

#include "Renderer/RenderDirectionalLightObject.h"

#include "Renderer/RenderSystem.h"
#include "Renderer/RenderManager.h"

PointLightComponent::PointLightComponent () :
	_range (1.0f)
{

}

void PointLightComponent::Awake ()
{
	_renderLightObject = new RenderPointLightObject ();

	Resource<Model> model = Primitive::Instance ()->Create (Primitive::Type::SPHERE);
	Resource<ModelView> modelView = RenderSystem::LoadModel (model);

	_renderLightObject->SetTransform (_parent->GetTransform ());
	_renderLightObject->SetModelView (modelView);
	_renderLightObject->SetLightColor (_color);
	_renderLightObject->SetLightIntensity (_intensity);
	_renderLightObject->SetShadowCasting (_castShadows);
	_renderLightObject->SetActive (_parent->IsActive ());

	RenderLightObject::Shadow lightShadow;

	lightShadow.resolution = _shadowInformation.resolution;
	lightShadow.cascadesCount = _shadowInformation.cascadesCount;
	lightShadow.bias = _shadowInformation.bias;

	_renderLightObject->SetLightShadow (lightShadow);

	auto renderLightObject = (RenderPointLightObject*) _renderLightObject;
	renderLightObject->SetLightRange (_range);

	RenderManager::Instance ()->AttachRenderPointLightObject (renderLightObject);
}

void PointLightComponent::Update ()
{
	if (_parent->GetTransform ()->GetScale ().x < 2.0f) {
		_parent->GetTransform ()->SetScale (glm::vec3 (_range));
	}
}

void PointLightComponent::OnAttachedToScene ()
{
	if (_renderLightObject == nullptr) {
		return;
	}

	auto renderLightObject = (RenderPointLightObject*) _renderLightObject;
	RenderManager::Instance ()->AttachRenderPointLightObject (renderLightObject);
}

void PointLightComponent::OnDetachedFromScene ()
{
	if (_renderLightObject == nullptr) {
		return;
	}

	auto renderLightObject = (RenderPointLightObject*) _renderLightObject;
	RenderManager::Instance ()->DetachRenderPointLightObject (renderLightObject);
}

float PointLightComponent::GetRange () const
{
	return _range;
}

void PointLightComponent::SetRange (float range)
{
	_range = range;

	auto renderLightObject = (RenderPointLightObject*) _renderLightObject;
	renderLightObject->SetLightRange (_range);

	/*
	 * Set light volume scale based on light distance
	*/

	_parent->GetTransform ()->SetScale (glm::vec3 (_range));
}
