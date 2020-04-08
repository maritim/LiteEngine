#ifndef COMPONENTWIDGETMANAGER_H
#define COMPONENTWIDGETMANAGER_H

#include "Core/Singleton/Singleton.h"

#include "ComponentWidget.h"

#define REGISTER_COMPONENT_WIDGET(COMPONENT_WIDGET) static ComponentWidgetRegister<COMPONENT_WIDGET> dummyWidget (#COMPONENT_WIDGET);

class ComponentWidgetManager : public Singleton<ComponentWidgetManager>
{
	friend Singleton<ComponentWidgetManager>;

private:
	std::map<std::string, ComponentWidget*> _widgets;

public:
	ComponentWidget* GetComponentWidget (const std::string& name);

	void RegisterComponentWidget (const std::string& name, ComponentWidget*);
private:
	ComponentWidgetManager ();
	~ComponentWidgetManager ();
	ComponentWidgetManager (const ComponentWidgetManager&);
	ComponentWidgetManager& operator=(const ComponentWidgetManager&);
};

template <class ComponentWidgetType>
class ComponentWidgetRegister : public Object
{
public:
	ComponentWidgetRegister (const std::string& name)
	{
		ComponentWidgetManager::Instance ()->RegisterComponentWidget (name, new ComponentWidgetType ());
	}
};

#endif