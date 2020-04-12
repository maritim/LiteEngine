#include "DirectionalLightComponent.h"

#include "Utils/Primitives/Primitive.h"

#include "Renderer/RenderDirectionalLightObject.h"

#include "Renderer/RenderSystem.h"
#include "Renderer/RenderManager.h"

void DirectionalLightComponent::Awake ()
{
	_renderLightObject = new RenderDirectionalLightObject ();

	Resource<Model> model = Primitive::Instance ()->Create (Primitive::Type::QUAD);
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

	auto renderLightObject = (RenderDirectionalLightObject*) _renderLightObject;

	RenderManager::Instance ()->SetRenderDirectionalLightObject (renderLightObject);
}

void DirectionalLightComponent::OnAttachedToScene ()
{
	if (_renderLightObject == nullptr) {
		return;
	}

	auto renderLightObject = (RenderDirectionalLightObject*) _renderLightObject;

	RenderManager::Instance ()->SetRenderDirectionalLightObject (renderLightObject);
}

void DirectionalLightComponent::OnDetachedFromScene ()
{
	if (_renderLightObject == nullptr) {
		return;
	}

	RenderManager::Instance ()->SetRenderDirectionalLightObject (nullptr);
}
