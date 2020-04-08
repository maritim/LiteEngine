#ifndef SCENENODE_H
#define SCENENODE_H

#include "Core/Interfaces/Object.h"

class SceneNode : public Object
{
protected:
	Transform* _parent;
	std::set<Transform*> _children;

public:
	void SetParent (Transform* parent);
	void DetachParent ();

	Transform* GetParent () const;

	std::size_t GetChildrenCount () const;

	MULTIPLE_CONTAINER_TEMPLATE (set)
};

MULTIPLE_CONTAINER_SPECIALIZATION (set, Transform*, Transform, _children);

#endif