#include "CPUProfilerService.h"

void CPUProfilerService::StartFrame ()
{
	ProfilerService::StartFrame ();

	auto startCurrentFrame = std::chrono::high_resolution_clock::now ();

	std::chrono::duration<float, std::milli> lastFrame = startCurrentFrame - _startFrame;
	_lastFrameTime = lastFrame.count ();

	_startFrame = startCurrentFrame;
}

std::chrono::time_point<std::chrono::high_resolution_clock> CPUProfilerService::GetStartFrame () const
{
	return _startFrame;
}

float CPUProfilerService::GetLastFrameTime () const
{
	return _lastFrameTime;
}
