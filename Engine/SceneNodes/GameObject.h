#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "SceneGraph/SceneObject.h"
#include "Mesh/Model.h"

class GameObject : public SceneObject
{
private:
	Model* _mesh;
public:
	GameObject ();

	void AttachMesh (Model* mesh);
	Model* GetMesh () const;

	void Update ();

	~GameObject ();
private:
	void DestroyCurrentMesh ();
};

#endif