#include "DirectionalLight.h"

#include "Utils/Primitives/Primitive.h"

#include "Renderer/RenderDirectionalLightObject.h"

#include "Renderer/RenderSystem.h"
#include "Renderer/RenderManager.h"

DirectionalLight::DirectionalLight ()
{
	_renderLightObject = new RenderDirectionalLightObject ();

	Resource<Model> model = Primitive::Instance ()->Create (Primitive::Type::QUAD);
	Resource<ModelView> modelView = RenderSystem::LoadModel (model);

	_renderLightObject->SetTransform (_transform);
	_renderLightObject->SetModelView (modelView);
}

void DirectionalLight::Update ()
{
	
}

void DirectionalLight::OnAttachedToScene ()
{
	auto renderLightObject = (RenderDirectionalLightObject*) _renderLightObject;

	RenderManager::Instance ()->SetRenderDirectionalLightObject (renderLightObject);
}

void DirectionalLight::OnDetachedFromScene ()
{
	RenderManager::Instance ()->SetRenderDirectionalLightObject (nullptr);
}
