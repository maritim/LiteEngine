#include "ComponentObject.h"

#include <algorithm>

#include "ComponentManager.h"

#include "SceneGraph/SceneObject.h"

ComponentObject::~ComponentObject ()
{
	for (auto component : _needRemoveComponents) {
		ComponentManager::Instance ()->Unregister (component);
		component->OnDetachedFromScene ();

		delete component;
	}

	_needRemoveComponents.clear ();

	for (auto component : _components) {
		ComponentManager::Instance ()->Unregister (component);
		component->OnDetachedFromScene ();

		delete component;
	}

	_components.clear ();
}

void ComponentObject::Update ()
{
	for (auto component : _needRemoveComponents) {
		auto it = std::find (_components.begin (), _components.end (), component);

		if (it != _components.end ()) {
			_components.erase (it);
		}

		ComponentManager::Instance ()->Unregister (component);
		component->OnDetachedFromScene ();

		delete component;
	}

	_needRemoveComponents.clear ();
}

void ComponentObject::AttachComponent (Component* component)
{
	component->SetParent ((SceneObject*) this);

	_components.push_back (component);

	ComponentManager::Instance ()->Register (component);
}

void ComponentObject::DetachComponent (Component* component)
{
	_needRemoveComponents.push_back (component);
}
