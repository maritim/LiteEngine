#ifndef STATSVIEW_H
#define STATSVIEW_H

#include "Systems/Components/Component.h"

#include <vector>

class StatsView : public Component
{
	DECLARE_COMPONENT(StatsView)

protected:
	float _lastFrameRate;
	float _timeElapsed;
	float _computeRange;
	bool _firstTime;
	std::vector<float> _frameRates;

public:
	void Start ();

	void Update ();
protected:
	void ShowStatisticsWindow ();
};

REGISTER_COMPONENT(StatsView)

#endif