#ifndef DEBUGVIEWCONTROLLER_H
#define DEBUGVIEWCONTROLLER_H

#include "Systems/Components/Component.h"
#include "Systems/Components/ComponentsFactory.h"

#include <vector>

class DebugViewController : public Component
{
protected:
	bool _continuousVoxelizationReset;
	bool _lastContinuousVoxelization;
	bool _lastAmbientOcclusionEnabled;

public:
	void Start ();

	void Update ();
protected:
	void ShowRenderingSettingsWindow ();
};

REGISTER_COMPONENT(DebugViewController)

#endif