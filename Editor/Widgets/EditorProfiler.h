#ifndef EDITORPROFILER_H
#define EDITORPROFILER_H

#include "EditorWidget.h"
#include "EditorManager.h"

#include <list>

#include "Debug/Profiler/Profiler.h"

enum ProfilerType
{
	CPUProfiler = 0,
	GPUProfiler
};

class EditorProfiler : public EditorWidget
{
protected:
	float _timeSpan;
	int _profilerType;

	/*
	 * Profiler CPU
	*/

	std::list<ProfilerEntry> _profilerCPUEvents;
	std::list<float> _profilerCPUFrameTimes;
	std::vector<ProfilerEntry> _profilerCPUStats;
	float _profilerCPUFrameTimeStat;
	float _totalCPUTime;

	/*
	 * Profiler GPU
	*/

	std::list<ProfilerEntry> _profilerGPUEvents;
	std::list<float> _profilerGPUFrameTimes;
	std::vector<ProfilerEntry> _profilerGPUStats;
	float _profilerGPUFrameTimeStat;
	float _totalGPUTime;

public:
	EditorProfiler ();

	void Show ();
protected:
	void UpdateProfiler ();
	void ShowProfiler ();

	void ShowProfilerSettings ();
	void ShowProfilerEvents (float profilerFrameTimeStat, const std::vector<ProfilerEntry>& profilerStats);
	void ShowEvent (std::size_t& pos, const std::vector<ProfilerEntry>& profilerStats);

	void UpdateProfilerStatistics (const std::vector<ProfilerEntry*>& profilerEntries, float lastFrameTime,
		float& totalTime, std::list<ProfilerEntry>& profilerEvents, std::list<float>& profilerFrameTimes,
		float& profilerFrameTimeStat, std::vector<ProfilerEntry>& profilerStats);
};

REGISTER_EDITOR_WIDGET(EditorProfiler)

#endif