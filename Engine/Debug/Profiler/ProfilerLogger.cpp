#include "Debug/Profiler/Profiler.h"

std::size_t ProfilerLogger::_nestDepth (0);
std::size_t ProfilerLogger::_currentEntryOrder (0);

ProfilerLogger::ProfilerLogger (const std::string& name) :
	_name (name),
	_startMoment (std::chrono::high_resolution_clock::now ()),
	_entryOrder ()
{
	/*
	 * TODO: Find a way to avoid usage of guards here.
	*/

	if (!Profiler::Instance ()->IsActive ()) {
		return;
	}

	if (Profiler::Instance ()->GetEvents ().size () == 0 && _nestDepth == 0) {
		_currentEntryOrder = 0;
	}

	_entryOrder = _currentEntryOrder;

	_nestDepth ++;
	_currentEntryOrder ++;
}

ProfilerLogger::~ProfilerLogger ()
{
	if (!Profiler::Instance ()->IsActive ()) {
		return;
	}

	std::chrono::time_point<std::chrono::high_resolution_clock> endMoment = std::chrono::high_resolution_clock::now ();

	std::chrono::duration<float, std::milli> duration = endMoment - _startMoment;

	ProfilerLoggerContainer container;
	container.Name = _name;
	container.Duration = duration.count ();
	container.NestDepth = _nestDepth;

	Profiler::Instance ()->AddEventAt (container, _entryOrder);

	_nestDepth --;
} 