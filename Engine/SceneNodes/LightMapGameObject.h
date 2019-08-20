#ifndef LIGHTMAPGAMEOBJECT_H
#define LIGHTMAPGAMEOBJECT_H

#include "GameObject.h"

class ENGINE_API LightMapGameObject : public GameObject
{
public:
	LightMapGameObject ();
	~LightMapGameObject ();

	void AttachMesh (const Resource<Model>& mesh);
};

#endif