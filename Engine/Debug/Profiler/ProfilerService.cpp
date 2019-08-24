#include "ProfilerService.h"

ProfilerEntry::ProfilerEntry () :
	Name (),
	Duration (0),
	StartTime (0),
	NestDepth (0)
{

}

ProfilerService::ProfilerService () :
	_isActive (false),
	_nextActive (false),
	_lastFrameQueue (),
	_currentFrameQueue ()
{

}

void ProfilerService::SetActive (bool isActive)
{
	_nextActive = isActive;
}

bool ProfilerService::IsActive () const
{
	return _isActive;
}

void ProfilerService::StartFrame ()
{
	_isActive = _nextActive;

	/*
	 * Clear last frame queue
	*/

	for (auto profilerEvent : _lastFrameQueue) {
		delete profilerEvent;
	}

	/*
	 *
	*/

	_lastFrameQueue.clear ();
	_lastFrameQueue.insert (_lastFrameQueue.begin(), 
		_currentFrameQueue.begin(), _currentFrameQueue.end());
	_currentFrameQueue.clear ();
}

void ProfilerService::PushEvent (const ProfilerEntry* entry)
{
	_currentFrameQueue.push_back ((ProfilerEntry*) entry);	
}

const std::vector<ProfilerEntry*>& ProfilerService::GetLastFrameEvents () const
{
	return _lastFrameQueue;
}

const std::vector<ProfilerEntry*>& ProfilerService::GetCurrentFrameEvents () const
{
	return _currentFrameQueue;
}

void ProfilerService::Clear ()
{
	_lastFrameQueue.clear ();
	_currentFrameQueue.clear ();
}