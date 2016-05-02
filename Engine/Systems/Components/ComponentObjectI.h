#ifndef COMPONENTOBJECTI_H
#define COMPONENTOBJECTI_H

#include "Component.h"

#include <vector>

class ComponentObjectI
{
protected:	
	std::vector<Component*> _components;

public:
	void AttachComponent (Component*);
	void DetachComponent (Component*);
};

#endif