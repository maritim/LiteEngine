#ifndef COMPONENTOBJECT_H
#define COMPONENTOBJECT_H

#include "Core/Interfaces/Object.h"

#include <vector>

#include "Component.h"

#include "Core/Iteration/MultipleContainer.h"

class ENGINE_API ComponentObject : public Object
{
protected:	
	std::vector<Component*> _components;
	std::vector<Component*> _needRemoveComponents;

public:
	~ComponentObject ();

	virtual void Update ();

	void AttachComponent (Component*);
	void DetachComponent (Component*);

	template<class T>
	T* GetComponent ();
	template<class T>
	std::vector<T*> GetComponents ();

	MULTIPLE_CONTAINER_TEMPLATE (vector)
};

template<class T>
T* ComponentObject::GetComponent ()
{
	T dummy;

	for (auto component : _components) {
		if (dummy.GetName () == component->GetName ()) {
			return (T*) component;
		}
	}

	return nullptr;
}

template<class T>
std::vector<T*> ComponentObject::GetComponents ()
{
	std::vector<T*> result;

	T dummy;

	for (auto component : _components) {
		if (dummy.GetName () == component->GetName ()) {
			result.push_back ((T*) component);
		}
	}

	return result;
}

MULTIPLE_CONTAINER_SPECIALIZATION (vector, Component*, ComponentObject, _components);

#endif