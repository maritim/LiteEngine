#include "ComponentObject.h"

#include <algorithm>

#include "ComponentManager.h"

#include "SceneGraph/SceneObject.h"

void ComponentObject::AttachComponent (Component* component)
{
	component->SetParent ((SceneObject*) this);

	_components.push_back (component);

	ComponentManager::Instance ()->Register (component);
}

void ComponentObject::DetachComponent (Component* component)
{
	auto it = std::find (_components.begin (), _components.end (), component);

	if (it == _components.end ()) {
		return ;
	}

	_components.erase (it);

	ComponentManager::Instance ()->Unregister (component);	
}
