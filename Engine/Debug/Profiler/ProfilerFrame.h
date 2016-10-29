#ifndef PROFILERFRAME_H
#define PROFILERFRAME_H

#include <cstddef>
#include <string>

#define PROFILER_FRAME ProfilerFrame profilerFrameTemporalObject;

#define PROFILER_OUTPUT_FILE "Profiler.prof"

class ProfilerFrame
{
private:
	static std::size_t _frameCount;

public:
	ProfilerFrame ();
	~ProfilerFrame ();
private:
	void LogFrame ();
};

#endif