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

	float _lastFrameRate;
	float _timeElapsed;
	float _computeRange;
	bool _firstTime;
	std::vector<float> _frameRates;

public:
	void Start ();

	void Update ();
protected:
	void ShowRenderingSettingsWindow ();
	void ShowStatisticsWindow ();
};

REGISTER_COMPONENT(DebugViewController)

#endif