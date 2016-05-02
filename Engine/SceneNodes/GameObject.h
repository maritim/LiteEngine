#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "SceneGraph/SceneObject.h"
#include "Systems/Components/ComponentObjectI.h"

#include "Mesh/Model.h"

class GameObject : public SceneObject, public ComponentObjectI
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