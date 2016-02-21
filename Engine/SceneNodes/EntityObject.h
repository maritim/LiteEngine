#ifndef ENTITYOBJECT_H
#define ENTITYOBJECT_H

#include "SceneGraph/SceneObject.h"

class EntityObject : public SceneObject
{
public:
	EntityObject();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	~EntityObject();
};

#endif