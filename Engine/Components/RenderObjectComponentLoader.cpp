#include "RenderObjectComponentLoader.h"

#include "Components/RenderObjectComponent.h"

#include "SceneNodes/SceneLayer.h"

#include "Resources/Resources.h"

#include "Renderer/RenderSystem.h"

#include "Renderer/RenderAnimationObject.h"

Component* RenderObjectComponentLoader::Load (TiXmlElement* xmlElem)
{
	std::string stage = xmlElem->Attribute ("stage");
	std::string layer = xmlElem->Attribute ("layer");

	RenderObjectComponent* renderObjectComponent = new RenderObjectComponent ();

	RenderObject* renderObject = GetRenderObject (layer);

	renderObjectComponent->SetRenderObject (renderObject);

	if (stage == "deferred") {
		renderObject->SetRenderStage (RenderStage::RENDER_STAGE_DEFERRED);
	} else {
		renderObject->SetRenderStage (RenderStage::RENDER_STAGE_FORWARD);
	}

	renderObject->SetPriority (1);

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content) 
	{
		std::string name = content->Value ();

		if (name == "Model") {
			ProcessModel (content, renderObjectComponent);
		}

		content = content->NextSiblingElement ();
	}

	return renderObjectComponent;
}

void RenderObjectComponentLoader::ProcessModel (TiXmlElement* xmlElem, RenderObjectComponent* renderObjectComponent)
{
	std::string path = xmlElem->Attribute ("path");

	RenderObject* renderObject = renderObjectComponent->GetRenderObject ();

	Resource<Model> model = nullptr;

	Resource<ModelView> modelView = nullptr;

	if (renderObject->GetSceneLayers () & SceneLayer::ANIMATION) {
		model = Resources::LoadAnimatedModel (path);

		modelView = RenderSystem::LoadAnimationModel (model);
	} else {
		model = Resources::LoadModel (path);

		if (renderObject->GetSceneLayers () & SceneLayer::STATIC) {
			modelView = RenderSystem::LoadModel (model);
		}
		else if (renderObject->GetSceneLayers () & SceneLayer::NORMAL_MAP) {
			modelView = RenderSystem::LoadNormalMapModel (model);
		}
		else if (renderObject->GetSceneLayers () & SceneLayer::LIGHT_MAP) {
			modelView = RenderSystem::LoadLightMapModel (model);
		}
	}

	renderObjectComponent->SetModel (model);

	renderObject->SetModelView (modelView);
}

RenderObject* RenderObjectComponentLoader::GetRenderObject (const std::string& layer)
{
	RenderObject* renderObject = nullptr;

	if (layer == "animated") {
		renderObject = new RenderAnimationObject ();

		renderObject->SetSceneLayers (SceneLayer::ANIMATION);
	} else {
		renderObject = new RenderObject ();

		if (layer == "static") {
			renderObject->SetSceneLayers (SceneLayer::STATIC);
		}
		else if (layer == "normalMap") {
			renderObject->SetSceneLayers (SceneLayer::NORMAL_MAP);
		}
		else if (layer == "lightMap") {
			renderObject->SetSceneLayers (SceneLayer::LIGHT_MAP);
		}

		renderObject->SetSceneLayers (renderObject->GetSceneLayers () | SceneLayer::STATIC);		
	}

	return renderObject;
}
