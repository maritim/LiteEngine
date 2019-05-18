#ifndef RENDERER_H
#define RENDERER_H

#include "SceneGraph/Transform.h"

class ENGINE_API Renderer
{
public:
	enum StageType {DEFERRED_STAGE, FORWARD_STAGE};

protected:
	StageType _stage;
	std::size_t _priority;
	Transform* _transform;
public:
	Renderer ();
	Renderer (Transform* transform);
	virtual ~Renderer ();

	virtual void Draw ();
	virtual void DrawGeometry ();

	StageType GetStageType () const;
	void SetStageType (StageType stageType);

	std::size_t GetPriority () const;
	void SetPriority (std::size_t priority);

	Transform* GetTransform () const;
	void SetTransform (Transform* transform);
};

#endif