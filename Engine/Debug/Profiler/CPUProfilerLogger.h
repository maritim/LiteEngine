#ifndef CPUPROFILERLOGGER_H
#define CPUPROFILERLOGGER_H

#include "Core/Interfaces/Object.h"

#include <string>
#include <chrono>

#define PROFILER_LOGGER(NAME) CPUProfilerLogger profilerLoggerTemporalObject(NAME); 

struct ProfilerEntry;

class CPUProfilerLogger : public Object
{
protected:
	static std::size_t _nestDepth;

	std::chrono::time_point<std::chrono::high_resolution_clock> _startMoment;

	ProfilerEntry* _entry;

public:
	CPUProfilerLogger (const std::string&);
	~CPUProfilerLogger ();
};

#endif