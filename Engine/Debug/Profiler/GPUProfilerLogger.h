#ifndef GPUPROFILERLOGGER_H
#define GPUPROFILERLOGGER_H

#include "Core/Interfaces/Object.h"

#include <string>

#define PROFILER_GPU_LOGGER(NAME) GPUProfilerLogger gpuProfilerLoggerTemporalObject(NAME); 

struct GPUProfilerEntry;

class GPUProfilerLogger : public Object
{
protected:
	static std::size_t _nestDepth;

	GPUProfilerEntry* _entry;

public:
	GPUProfilerLogger (const std::string&);
	~GPUProfilerLogger ();
};

#endif