#include "CPUProfilerLogger.h"

#include "Debug/Profiler/Profiler.h"

std::size_t CPUProfilerLogger::_nestDepth (0);

CPUProfilerLogger::CPUProfilerLogger (const std::string& name) :
	_startMoment (std::chrono::high_resolution_clock::now ()),
	_entry (nullptr)
{
	CPUProfilerService* profilerService = Profiler::Instance ()->GetCPUProfilerService ();

	/*
	 * TODO: Find a way to avoid usage of guards here.
	*/

	if (!profilerService->IsActive ()) {
		return;
	}

	_entry = new ProfilerEntry ();

	_entry->Name = name;
	_entry->NestDepth = _nestDepth;

	profilerService->PushEvent (_entry);

	_nestDepth ++;
}

CPUProfilerLogger::~CPUProfilerLogger ()
{
	CPUProfilerService* profilerService = Profiler::Instance ()->GetCPUProfilerService ();

	if (!profilerService->IsActive ()) {
		return;
	}

	std::chrono::time_point<std::chrono::high_resolution_clock> endMoment = std::chrono::high_resolution_clock::now ();

	std::chrono::duration<float, std::milli> startTime = _startMoment - profilerService->GetStartFrame ();
	std::chrono::duration<float, std::milli> duration = endMoment - _startMoment;

	_entry->Duration = duration.count ();
	_entry->StartTime = startTime.count ();

	_nestDepth --;
} 