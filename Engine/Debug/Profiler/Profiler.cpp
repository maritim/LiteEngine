#include "Profiler.h"

Profiler::Profiler () :
	_profilerCPUService (new CPUProfilerService ()),
	_profilerGPUService (new GPUProfilerService ())
{

}

Profiler::~Profiler ()
{

}

CPUProfilerService* Profiler::GetCPUProfilerService ()
{
	return _profilerCPUService;
}

GPUProfilerService* Profiler::GetGPUProfilerService ()
{
	return _profilerGPUService;
}
