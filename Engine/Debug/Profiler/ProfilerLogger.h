#ifndef PROFILERLOGGER_H
#define PROFILERLOGGER_H

#include <string>
#include <chrono>

#include "Core/Interfaces/Object.h"
#include "Debug/Profiler/Profiler.h"

#define PROFILER_LOGGER(NAME) ProfilerLogger profilerLoggerTemporalObject(#NAME); 

class ProfilerLogger
{
private:
	static std::size_t _nestDepth;
	static std::size_t _currentEntryOrder;

	std::string _name;
	std::chrono::time_point<std::chrono::high_resolution_clock> _startMoment;
	std::size_t _entryOrder;

public:
	ProfilerLogger (const std::string&);
	~ProfilerLogger ();
};

#endif