#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "Core/Interfaces/Object.h"

#include "SceneGraph/Transform.h"

#include "Core/Resources/Resource.h"
#include "RenderViews/ModelView.h"
#include "Renderer/RenderStage.h"
#include "Systems/Collision/Collider.h"

#include "Renderer/PipelineAttribute.h"

class RenderObject : public Object
{
protected:
	Transform* _transform;
	Resource<ModelView> _modelView;
	RenderStage _renderStage;
	int _sceneLayers;
	int _priority;
	Collider* _collider;
	bool _isActive;

	std::vector<PipelineAttribute> _attributes;

public:
	RenderObject ();

	void SetTransform (Transform* transform);
	void SetModelView (const Resource<ModelView>& modelView);
	void SetRenderStage (RenderStage renderStage);
	void SetSceneLayers (int sceneLayers);
	void SetPriority (int pritority);
	void SetCollider (Collider* collider);
	void SetActive (bool isEnabled);
	void SetAttributes (const std::vector<PipelineAttribute>& attributes);
	
	Transform* GetTransform () const;
	Resource<ModelView> GetModelView () const;
	RenderStage GetRenderStage () const;
	int GetSceneLayers () const;
	int GetPriority () const;
	Collider* GetCollider () const;
	bool IsActive () const;

	virtual void Draw ();
};

#endif