#ifndef RENDERER_H
#define RENDERER_H

#include "SceneGraph/Transform.h"

class Renderer
{
protected:
	std::size_t _priority;
	Transform* _transform;
public:
	Renderer ();
	Renderer (Transform* transform);
	virtual ~Renderer ();

	virtual void Draw ();

	std::size_t GetPriority () const;
	void SetPriority (std::size_t priority);

	Transform* GetTransform () const;
	void SetTransform (Transform* transform);
};

#endif