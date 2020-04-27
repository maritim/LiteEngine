#include "SpotLightComponent.h"

#include "Utils/Primitives/Primitive.h"

#include "Renderer/RenderSpotLightObject.h"

#include "Renderer/RenderSystem.h"
#include "Renderer/RenderManager.h"

#include "Utils/Extensions/MathExtend.h"

SpotLightComponent::SpotLightComponent () :
	_cutoff (0.0f),
	_outerCutoff (30.0f)
{

}

void SpotLightComponent::Awake ()
{
	_renderLightObject = new RenderSpotLightObject ();

	Resource<Model> model = Primitive::Instance ()->Create (Primitive::Type::CONE);
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

	auto renderLightObject = (RenderSpotLightObject*) _renderLightObject;
	renderLightObject->SetLightRange (_range);
	renderLightObject->SetLightSpotCutoff (_cutoff);
	renderLightObject->SetLightSpotOuterCutoff (_outerCutoff);

	RenderManager::Instance ()->AttachRenderSpotLightObject (renderLightObject);
}

void SpotLightComponent::Update ()
{
	if (_parent->GetTransform ()->GetScale ().x < 2.0f) {
		_parent->GetTransform ()->SetScale (glm::vec3 (_range));
	}
}

void SpotLightComponent::OnAttachedToScene ()
{
	if (_renderLightObject == nullptr) {
		return;
	}

	auto renderLightObject = (RenderSpotLightObject*) _renderLightObject;
	RenderManager::Instance ()->AttachRenderSpotLightObject (renderLightObject);
}

void SpotLightComponent::OnDetachedFromScene ()
{
	if (_renderLightObject == nullptr) {
		return;
	}

	auto renderLightObject = (RenderSpotLightObject*) _renderLightObject;
	RenderManager::Instance ()->DetachRenderSpotLightObject (renderLightObject);
}

float SpotLightComponent::GetSpotCutoff () const
{
	return _cutoff;
}

float SpotLightComponent::GetSpotOuterCutoff () const
{
	return _outerCutoff;
}

void SpotLightComponent::SetCutoff (float spotCutoff)
{
	_cutoff = spotCutoff;

	auto renderLightObject = (RenderSpotLightObject*) _renderLightObject;
	renderLightObject->SetLightSpotCutoff (_cutoff);

	UpdateTransform ();
}

void SpotLightComponent::SetOuterCutoff (float spotOuterCutoff)
{
	_outerCutoff = spotOuterCutoff;

	auto renderLightObject = (RenderSpotLightObject*) _renderLightObject;
	renderLightObject->SetLightSpotOuterCutoff (_outerCutoff);

	UpdateTransform ();
}

void SpotLightComponent::UpdateTransform ()
{
	/*
	 * Set light volume scale based on light distance
	*/

	glm::vec3 scale (0.0f);

	scale.z = _range;
	scale.x = scale.y = _range * std::tan (DEG2RAD * _outerCutoff) * 2;

	_parent->GetTransform ()->SetScale (scale);
}
