#include "RenderObjectComponent.h"

#include "Renderer/RenderAnimationObject.h"
#include "Renderer/RenderSystem.h"
#include "Renderer/RenderManager.h"

#include "SceneNodes/SceneLayer.h"

RenderObjectComponent::RenderObjectComponent () :
	_renderStage ((int) RenderStage::RENDER_STAGE_DEFERRED),
	_layer (1),
	_model (nullptr),
	_renderObject (nullptr)
{

}

RenderObjectComponent::~RenderObjectComponent ()
{
	delete _renderObject;
}

void RenderObjectComponent::Awake ()
{
	SetLayer (_layer);

	_renderObject->SetTransform (_parent->GetTransform ());
	_renderObject->SetRenderStage ((RenderStage) _renderStage);
	_renderObject->SetActive (_parent->IsActive ());
}

void RenderObjectComponent::Update ()
{
	if (_renderObject == nullptr) {
		return;
	}

	_renderObject->Update ();
}

void RenderObjectComponent::SetActive (bool isActive)
{
	if (_renderObject == nullptr) {
		return;
	}

	_renderObject->SetActive (isActive);
}

void RenderObjectComponent::OnAttachedToScene ()
{
	if (_renderObject == nullptr) {
		return;
	}

	if (_model == nullptr) {
		return;
	}

	RenderManager::Instance ()->AttachRenderObject (_renderObject);
}

void RenderObjectComponent::OnDetachedFromScene ()
{
	if (_renderObject == nullptr) {
		return;
	}

	RenderManager::Instance ()->DetachRenderObject (_renderObject);
}

void RenderObjectComponent::SetModel (const Resource<Model>& model)
{
	_model = model;

	if (_model == nullptr) {
		return;
	}

	Resource<ModelView> modelView = nullptr;

	if (_layer & SceneLayer::ANIMATION) {
		modelView = RenderSystem::LoadAnimationModel (_model);
	}

	if (_layer & SceneLayer::STATIC) {
		modelView = RenderSystem::LoadModel (_model);
	}

	if (_layer & SceneLayer::NORMAL_MAP) {
		modelView = RenderSystem::LoadNormalMapModel (_model);
	}

	if (_layer & SceneLayer::LIGHT_MAP) {
		modelView = RenderSystem::LoadLightMapModel (_model);
	}

	_renderObject->SetModelView (modelView);

	auto& boundingBox = _model->GetBoundingBox ();
	AABBVolume volume (
		glm::vec3 (boundingBox.xmin, boundingBox.ymin, boundingBox.zmin),
		glm::vec3 (boundingBox.xmax, boundingBox.ymax, boundingBox.zmax)
	);

	_renderObject->SetBoundingBox (volume);
}

void RenderObjectComponent::SetRenderStage (int renderStage)
{
	_renderStage = renderStage;

	if (_renderObject == nullptr) {
		return;
	}

	_renderObject->SetRenderStage ((RenderStage) _renderStage);
}

void RenderObjectComponent::SetLayer (int sceneLayers)
{
	_layer = sceneLayers;

	OnDetachedFromScene ();

	delete _renderObject;

	if (_layer & SceneLayer::ANIMATION) {
		_renderObject = new RenderAnimationObject ();

		_renderObject->SetSceneLayers (_layer);

		auto renderObject = (RenderAnimationObject*) _renderObject;
		renderObject->SetAnimationModel (_model);
	} else {
		_renderObject = new RenderObject ();

		_renderObject->SetSceneLayers (_layer | SceneLayer::STATIC);
	}

	/*
	 * Set render object model view
	*/

	SetModel (_model);

	OnAttachedToScene ();
}

const Resource<Model>& RenderObjectComponent::GetModel () const
{
	return _model;
}

const AABBVolume& RenderObjectComponent::GetBoundingBox () const
{
	return _renderObject->GetBoundingBox ();
}

#include "Systems/GUI/Gizmo/Gizmo.h"

void RenderObjectComponent::OnGizmo ()
{
	if (_renderObject == nullptr) {
		return;
	}

	auto& aabbData = _renderObject->GetBoundingBox ();

	glm::vec3 sub = aabbData.maxVertex - aabbData.minVertex;

	Gizmo::DrawLine (aabbData.minVertex, aabbData.minVertex + glm::vec3 (sub.x, 0, 0) , Color::White);
	Gizmo::DrawLine (aabbData.minVertex, aabbData.minVertex + glm::vec3 (0, sub.y, 0) , Color::White);
	Gizmo::DrawLine (aabbData.minVertex, aabbData.minVertex + glm::vec3 (0, 0, sub.z) , Color::White);
	Gizmo::DrawLine (aabbData.maxVertex, aabbData.maxVertex - glm::vec3 (sub.x, 0, 0) , Color::White);
	Gizmo::DrawLine (aabbData.maxVertex, aabbData.maxVertex - glm::vec3 (0, sub.y, 0) , Color::White);
	Gizmo::DrawLine (aabbData.maxVertex, aabbData.maxVertex - glm::vec3 (0, 0, sub.z) , Color::White);
	Gizmo::DrawLine (aabbData.minVertex + glm::vec3 (sub.x, 0, 0), aabbData.maxVertex - glm::vec3 (0, sub.y, 0) , Color::White);
	Gizmo::DrawLine (aabbData.minVertex + glm::vec3 (sub.x, 0, 0), aabbData.maxVertex - glm::vec3 (0, 0, sub.z) , Color::White);
	Gizmo::DrawLine (aabbData.minVertex + glm::vec3 (0, sub.y, 0), aabbData.maxVertex - glm::vec3 (0, 0, sub.z) , Color::White);
	Gizmo::DrawLine (aabbData.minVertex + glm::vec3 (0, sub.y, 0), aabbData.maxVertex - glm::vec3 (sub.x, 0, 0) , Color::White);
	Gizmo::DrawLine (aabbData.minVertex + glm::vec3 (0, 0, sub.z), aabbData.maxVertex - glm::vec3 (sub.x, 0, 0) , Color::White);
	Gizmo::DrawLine (aabbData.minVertex + glm::vec3 (0, 0, sub.z), aabbData.maxVertex - glm::vec3 (0, sub.y, 0) , Color::White);
}
