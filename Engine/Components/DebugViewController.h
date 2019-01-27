#ifndef DEBUGVIEWCONTROLLER_H
#define DEBUGVIEWCONTROLLER_H

#include "Systems/Components/Component.h"
#include "Systems/Components/ComponentsFactory.h"

class DebugViewController : public Component
{
public:
	void Start ();

	void Update ();
};

REGISTER_COMPONENT(DebugViewController)

#endif