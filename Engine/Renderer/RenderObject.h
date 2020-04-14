#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "Core/Interfaces/Object.h"

#include "SceneGraph/Transform.h"

#include "Core/Resources/Resource.h"
#include "RenderViews/ModelView.h"
#include "Core/Intersections/AABBVolume.h"
#include "Renderer/RenderStage.h"

#include "Renderer/PipelineAttribute.h"

class RenderObject : public Object
{
protected:
	const Transform* _transform;
	Resource<ModelView> _modelView;
	RenderStage _renderStage;
	int _sceneLayers;
	int _priority;
	bool _isActive;

	AABBVolume _modelSpaceBoundingBox;
	AABBVolume _worldSpaceBoundingBox;

	std::vector<PipelineAttribute> _attributes;

public:
	RenderObject ();

	void Update ();

	void SetTransform (const Transform* transform);
	void SetModelView (const Resource<ModelView>& modelView);
	void SetBoundingBox (const AABBVolume& boundingBox);
	void SetRenderStage (RenderStage renderStage);
	void SetSceneLayers (int sceneLayers);
	void SetPriority (int pritority);
	void SetActive (bool isEnabled);
	void SetAttributes (const std::vector<PipelineAttribute>& attributes);

	const Transform* GetTransform () const;
	Resource<ModelView> GetModelView () const;
	const AABBVolume& GetBoundingBox () const;
	RenderStage GetRenderStage () const;
	int GetSceneLayers () const;
	int GetPriority () const;
	bool IsActive () const;

	virtual void Draw ();
	virtual void DrawGeometry ();
protected:
	void UpdateBoundingBox ();
};

#endif