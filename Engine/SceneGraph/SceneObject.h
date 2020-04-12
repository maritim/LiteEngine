#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Core/Interfaces/Object.h"
#include "Systems/Components/ComponentObject.h"

#include <string>

#include "Transform.h"

class Transform;

class ENGINE_API SceneObject : public ComponentObject
{
protected:
	std::string _name;
	std::size_t _instanceID;
	Transform* _transform;
	int _sceneLayers;
	bool _isActive;

public:
	SceneObject ();
	
	std::string GetName () const;
	std::size_t GetInstanceID () const;
	bool IsActive () const;

	void SetName (const std::string& name);
	void SetInstanceID (std::size_t instanceID);
	virtual void SetActive (bool isActive);

	Transform* GetTransform () const;
	int GetLayers () const;

	virtual void Update ();

	virtual void OnAttachedToScene ();
	virtual void OnDetachedFromScene ();

	virtual ~SceneObject ();
};

#endif