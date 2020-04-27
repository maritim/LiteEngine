#include "ComponentManager.h"

#include <algorithm>

ComponentManager::ComponentManager ()
{

}

ComponentManager::~ComponentManager ()
{

}

void ComponentManager::Update ()
{
	for (auto component : _needRemoveComponents) {
		auto it = std::find (_components.begin (), _components.end (), component);

		if (it != _components.end ()) {
			_components.erase (it);
		}
	}

	_needRemoveComponents.clear ();

	for (auto component : _components) {
		component->Update ();
	}

	for (auto component : _newComponents) {
		component->Start ();

		_components.push_back (component);
	}

	_newComponents.clear ();
}

void ComponentManager::Register (Component* component)
{
	_newComponents.push_back (component);

	component->Awake ();
}

void ComponentManager::Unregister (Component* component)
{
	_needRemoveComponents.push_back (component);
}
