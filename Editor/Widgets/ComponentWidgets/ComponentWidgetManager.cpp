#include "ComponentWidgetManager.h"

ComponentWidgetManager::ComponentWidgetManager ()
{

}

ComponentWidgetManager::~ComponentWidgetManager ()
{
	for (auto componentWidget : _widgets) {
		delete componentWidget.second;
	}
}

ComponentWidget* ComponentWidgetManager::GetComponentWidget (const std::string& name)
{
	std::map<std::string, ComponentWidget*>::iterator it = _widgets.find (name);

	if (it == _widgets.end ()) {
		return nullptr;
	}

	return it->second;
}

void ComponentWidgetManager::RegisterComponentWidget (const std::string& name, ComponentWidget* componentWidget)
{
	_widgets.insert (std::pair<std::string, ComponentWidget*> (name, componentWidget));
}
