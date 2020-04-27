#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <vector>

#include "Component.h"

class ComponentManager : public Singleton<ComponentManager>
{
	friend Singleton<ComponentManager>;

private:
	std::vector<Component*> _components;
	std::vector<Component*> _newComponents;
	std::vector<Component*> _needRemoveComponents;

public:
	void Update ();

	void Register (Component*);
	void Unregister (Component*);
private:
	ComponentManager ();
	~ComponentManager ();
	ComponentManager (const ComponentManager&);
	ComponentManager& operator=(const ComponentManager&);
};

#endif