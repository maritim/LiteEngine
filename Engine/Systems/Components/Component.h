#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core/Interfaces/Object.h"

#include "Core/ObjectsFactory/ObjectsFactory.h"

#define DECLARE_COMPONENT(COMPONENT) \
public: \
	std::string GetName () const \
	{ \
		return #COMPONENT; \
	}

#define ATTRIBUTE(a,b)

class ComponentObject;
class SceneObject;

class ENGINE_API Component : public Object
{
	friend ComponentObject;

protected:
	SceneObject* _parent;

public:
	Component ();
	virtual ~Component ();

	virtual void Awake ();
	virtual void Start ();

	virtual void Update ();
	virtual void LateUpdate ();

	virtual void SetActive (bool isActive);

	virtual void OnAttachedToScene ();
	virtual void OnDetachedFromScene ();

	virtual void OnGizmo ();

	virtual std::string GetName () const = 0;
protected:
	void SetParent (SceneObject* parent);
};

#define REGISTER_COMPONENT(COMPONENT) static RegisterObject<Component, COMPONENT> dummy##COMPONENT (#COMPONENT);

#endif