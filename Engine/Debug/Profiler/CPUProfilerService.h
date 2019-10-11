#ifndef CPUPROFILERSERVICE_H
#define CPUPROFILERSERVICE_H

#include "ProfilerService.h"

#include <chrono>

#include "CPUProfilerLogger.h"

class ENGINE_API CPUProfilerService : public ProfilerService
{
protected:
	std::chrono::time_point<std::chrono::high_resolution_clock> _startFrame;

	float _lastFrameTime;

public:
	void StartFrame ();

	std::chrono::time_point<std::chrono::high_resolution_clock> GetStartFrame () const;
	float GetLastFrameTime () const;
};

#endif