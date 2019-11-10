#include "SpotLight.h"

#include "Utils/Primitives/Primitive.h"

#include "Renderer/RenderSpotLightObject.h"

#include "Renderer/RenderSystem.h"
#include "Renderer/RenderManager.h"

#include "Utils/Extensions/MathExtend.h"

SpotLight::SpotLight () :
	_spotCutoff (0.0f),
	_spotOuterCutoff (0.0f)
{
	delete _renderLightObject;
	_renderLightObject = new RenderSpotLightObject ();

	Resource<Model> model = Primitive::Instance ()->Create (Primitive::Type::CONE);
	Resource<ModelView> modelView = RenderSystem::LoadModel (model);

	_renderLightObject->SetTransform (_transform);
	_renderLightObject->SetModelView (modelView);
}

float SpotLight::GetSpotCutoff () const
{
	return _spotCutoff;
}

float SpotLight::GetSpotOuterCutoff () const
{
	return _spotOuterCutoff;
}

void SpotLight::SetSpotCutoff (float spotCutoff)
{
	_spotCutoff = spotCutoff;

	auto renderLightObject = (RenderSpotLightObject*) _renderLightObject;
	renderLightObject->SetLightSpotCutoff (_spotCutoff);

	UpdateTransform ();
}

void SpotLight::SetSpotOuterCutoff (float spotOuterCutoff)
{
	_spotOuterCutoff = spotOuterCutoff;

	auto renderLightObject = (RenderSpotLightObject*) _renderLightObject;
	renderLightObject->SetLightSpotOuterCutoff (_spotOuterCutoff);

	UpdateTransform ();
}

void SpotLight::OnAttachedToScene ()
{
	auto renderLightObject = (RenderSpotLightObject*) _renderLightObject;
	RenderManager::Instance ()->AttachRenderSpotLightObject (renderLightObject);
}

void SpotLight::OnDetachedFromScene ()
{
	auto renderLightObject = (RenderSpotLightObject*) _renderLightObject;
	RenderManager::Instance ()->DetachRenderSpotLightObject (renderLightObject);
}

void SpotLight::UpdateTransform ()
{
	/*
	 * Set light volume scale based on light distance
	*/

	glm::vec3 scale (0.0f);

	scale.z = _range;
	scale.x = scale.y = _range * std::tan (DEG2RAD * _spotOuterCutoff) * 2;

	_transform->SetScale (scale);
}
