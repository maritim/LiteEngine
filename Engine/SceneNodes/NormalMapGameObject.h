#ifndef NORMALMAPGAMEOBJECT_H
#define NORMALMAPGAMEOBJECT_H

#include "GameObject.h"

class ENGINE_API NormalMapGameObject : public GameObject
{
protected:

public:
	NormalMapGameObject ();
	~NormalMapGameObject ();

	void AttachMesh (const Resource<Model>& mesh);
};

#endif