#include "ComponentManager.h"

#include <algorithm>

ComponentManager::ComponentManager ()
{

}

ComponentManager::~ComponentManager ()
{
	Clear ();
}

void ComponentManager::Update ()
{
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
	auto it = std::find (_components.begin (), _components.end (), component);

	if (it == _components.end ()) {
		return ;
	}

	delete *it;

	_components.erase (it);
}

void ComponentManager::Clear ()
{
	for (auto component : _components) {
		delete component;
	}

	for (auto component : _newComponents) {
		delete component;
	}

	_components.clear ();
	_components.shrink_to_fit ();

	_newComponents.clear ();
	_newComponents.shrink_to_fit ();
}