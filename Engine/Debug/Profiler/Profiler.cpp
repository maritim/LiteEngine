#include "Profiler.h"

#include "Arguments/ArgumentsAnalyzer.h"

ProfilerLoggerContainer::ProfilerLoggerContainer () :
	Name (),
	Duration (0),
	NestDepth (0)
{

}

Profiler::Profiler () :
	_isActive (ArgumentsAnalyzer::Instance ()->GetArgument (PROFILER_ARGUMENT_FLAG) != nullptr),
	_eventsQueue ()
{

}

Profiler::~Profiler ()
{

}

bool Profiler::IsActive () const
{
	return _isActive;
}

void Profiler::AddEventAt (const ProfilerLoggerContainer& eventContainer, std::size_t index)
{
	if (_eventsQueue.size () <= index) {
		Allocate (_eventsQueue.size () - index + 1)
	}

	_eventsQueue [index] = eventContainer;
}

const std::vector<ProfilerLoggerContainer>& Profiler::GetEvents () const
{
	return _eventsQueue;
}

void Profiler::Clear ()
{
	/*
	 * Clear stack by replacing it with an empty new stack
	*/
	 
	_eventsQueue.clear ();
	_eventsQueue.shrink_to_fit ();
}

void Profiler::Allocate (std::size_t count)
{
	for (std::size_t i=0;i<count;i++) {
		_eventsQueue.push_back (ProfilerLoggerContainer ());
	}
}