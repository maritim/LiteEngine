#ifndef RENDEROBJECTCOMPONENTWIDGET_H
#define RENDEROBJECTCOMPONENTWIDGET_H

#include "ComponentWidget.h"
#include "ComponentWidgetManager.h"

class RenderObjectComponentWidget : public ComponentWidget
{
public:
	void Show (Component* component);
};

REGISTER_COMPONENT_WIDGET(RenderObjectComponentWidget)

#endif