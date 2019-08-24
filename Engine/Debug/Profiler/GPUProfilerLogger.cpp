#include "GPUProfilerLogger.h"

#include "Debug/Profiler/Profiler.h"

#include "Wrappers/OpenGL/GL.h"

std::size_t GPUProfilerLogger::_nestDepth (0);

GPUProfilerLogger::GPUProfilerLogger (const std::string& name) :
	_entry (nullptr)
{
	GPUProfilerService* profilerService = Profiler::Instance ()->GetGPUProfilerService ();

	/*
	 * TODO: Find a way to avoid usage of guards here.
	*/

	if (!profilerService->IsActive ()) {
		return;
	}

	_entry = new GPUProfilerEntry ();

	_entry->Name = name;
	_entry->NestDepth = _nestDepth;

	GL::GenQueries (2, _entry->TimeQueries);

	GL::QueryCounter (_entry->TimeQueries [0], GL_TIMESTAMP);

	profilerService->PushEvent (_entry);

	_nestDepth ++;
}

GPUProfilerLogger::~GPUProfilerLogger ()
{
	GPUProfilerService* profilerService = Profiler::Instance ()->GetGPUProfilerService ();

	if (!profilerService->IsActive ()) {
		return;
	}

	GL::QueryCounter (_entry->TimeQueries [1], GL_TIMESTAMP);

	_nestDepth --;
}
