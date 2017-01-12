#ifndef STATISTICSVIEW_H
#define STATISTICSVIEW_H

#include "Systems/Components/Component.h"
#include "Systems/Components/ComponentsFactory.h"

#include "SceneNodes/TextGUI.h"

class StatisticsView : public Component
{
protected:
	TextGUI* _textGUI;

public:
	void Start ();

	void Update ();
};

REGISTER_COMPONENT(StatisticsView)

#endif