#ifndef COMPONENTWIDGET_H
#define COMPONENTWIDGET_H

#include "Core/Interfaces/Object.h"

#include "Systems/Components/Component.h"

class ComponentWidget : public Object
{
public:
	virtual void Show (Component*) = 0;
};

#endif