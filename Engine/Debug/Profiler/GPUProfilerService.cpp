#include "GPUProfilerService.h"

#include "Profiler.h"

#include "Wrappers/OpenGL/GL.h"

GPUProfilerEntry::~GPUProfilerEntry ()
{
	GL::DeleteQueries (2, TimeQueries);
}

void GPUProfilerEntry::Evaluate ()
{
	unsigned long int startTime, finalTime;

	GL::GetQueryObjectui64v(TimeQueries[0], GL_QUERY_RESULT, &startTime);
	GL::GetQueryObjectui64v(TimeQueries[1], GL_QUERY_RESULT, &finalTime);

	StartTime = (startTime - Profiler::Instance ()->GetGPUProfilerService ()->GetStartTime ()) / 1000000.0f;
	Duration = (finalTime - startTime) / 1000000.0f;
}

GPUProfilerService::GPUProfilerService () :
	_startFrameQuery (0)
{
	GL::GenQueries (1, &_lastStartFrameQuery);
	GL::GenQueries (1, &_startFrameQuery);
}

GPUProfilerService::~GPUProfilerService ()
{
	GL::DeleteQueries (1, &_startFrameQuery);
	GL::DeleteQueries (1, &_lastStartFrameQuery);
}

void GPUProfilerService::StartFrame ()
{
	_isActive = _nextActive;

	/*
	 * Clear last frame queue
	*/

	for (auto profilerEvent : _lastFrameQueue2) {
		delete profilerEvent;
	}

	/*
	 *
	*/

	_lastFrameQueue2.clear ();
	_lastFrameQueue2.insert (_lastFrameQueue2.begin(), 
		_lastFrameQueue.begin(), _lastFrameQueue.end());
	_lastFrameQueue.clear ();
	_lastFrameQueue.insert (_lastFrameQueue.begin(), 
		_currentFrameQueue.begin(), _currentFrameQueue.end());
	_currentFrameQueue.clear ();

	unsigned long int currentStartFrameTime;
	GL::GetQueryObjectui64v(_lastStartFrameQuery, GL_QUERY_RESULT, &currentStartFrameTime);

	_lastFrameTime = (currentStartFrameTime - _lastStartFrameTime) / 1000000.0f;

	_lastStartFrameTime = currentStartFrameTime;

	for (auto profilerEvent : _lastFrameQueue2) {
		((GPUProfilerEntry*) profilerEvent)->Evaluate ();
	}

	std::swap (_lastStartFrameQuery, _startFrameQuery);
	GL::QueryCounter (_startFrameQuery, GL_TIMESTAMP);
}

const std::vector<ProfilerEntry*>& GPUProfilerService::GetLastFrameEvents () const
{
	return _lastFrameQueue2;
}

unsigned long int GPUProfilerService::GetStartTime () const
{
	return _lastStartFrameTime;
}

float GPUProfilerService::GetLastFrameTime () const
{
	return _lastFrameTime;
}
