#include "ComponentObjectI.h"

#include <algorithm>

#include "ComponentManager.h"

void ComponentObjectI::AttachComponent (Component* component)
{
	_components.push_back (component);

	ComponentManager::Instance ()->Register (component);
}

void ComponentObjectI::DetachComponent (Component* component)
{
	auto it = std::find (_components.begin (), _components.end (), component);

	if (it == _components.end ()) {
		return ;
	}

	_components.erase (it);

	ComponentManager::Instance ()->Unregister (component);	
}