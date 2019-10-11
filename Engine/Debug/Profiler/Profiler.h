#ifndef PROFILER_H
#define PROFILER_H

#include "Core/Singleton/Singleton.h"

#include "ProfilerService.h"

#include "CPUProfilerService.h"
#include "GPUProfilerService.h"

#define PROFILER_FRAME \
Profiler::Instance ()->GetCPUProfilerService ()->StartFrame (); \
Profiler::Instance ()->GetGPUProfilerService ()->StartFrame ();

class ENGINE_API Profiler : public Singleton<Profiler>
{
	friend Singleton<Profiler>;

	DECLARE_SINGLETON(Profiler)

private:
	CPUProfilerService* _profilerCPUService;
	GPUProfilerService* _profilerGPUService;

public:
	CPUProfilerService* GetCPUProfilerService ();
	GPUProfilerService* GetGPUProfilerService ();
private:
	Profiler ();
	~Profiler ();
	Profiler (const Profiler&);
	Profiler& operator=(const Profiler&);
};

#endif