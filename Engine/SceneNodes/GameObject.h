#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "SceneGraph/SceneObject.h"
#include "Systems/Components/ComponentObjectI.h"

#include "Mesh/Model.h"
#include "Core/Resources/Resource.h"

class ENGINE_API GameObject : public SceneObject, public ComponentObjectI
{
protected:
	Resource<Model> _mesh;
public:
	GameObject ();

	virtual void AttachMesh (const Resource<Model>& mesh);
	Resource<Model> GetMesh () const;

	void Update ();

	~GameObject ();
};

#endif