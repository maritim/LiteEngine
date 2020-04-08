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

public:
	void AttachComponent (Component*);
	void DetachComponent (Component*);

	template<class T>
	T* GetComponent ();

	MULTIPLE_CONTAINER_TEMPLATE (vector)
};

MULTIPLE_CONTAINER_SPECIALIZATION (vector, Component*, ComponentObject, _components);

#endif