#ifndef EDITORSTATS_H
#define EDITORSTATS_H

#include "EditorWidget.h"
#include "EditorManager.h"

class EditorStats : public EditorWidget
{
protected:
	float _lastFrameRate;
	float _timeElapsed;
	float _computeRange;
	bool _firstTime;
	std::vector<float> _frameRates;

public:
	EditorStats ();

	void Show ();
protected:
	void ShowStatistics ();
};

REGISTER_EDITOR_WIDGET(EditorStats)

#endif