#ifndef GPUPROFILERSERVICE_H
#define GPUPROFILERSERVICE_H

#include "ProfilerService.h"

#include "GPUProfilerLogger.h"

struct GPUProfilerEntry : ProfilerEntry
{
	unsigned int TimeQueries[2];

	~GPUProfilerEntry ();

	void Evaluate ();
};

class ENGINE_API GPUProfilerService : public ProfilerService
{
protected:
	std::vector<ProfilerEntry*> _lastFrameQueue2;

	uint64_t _lastStartFrameTime;
	unsigned int _lastStartFrameQuery;
	uint64_t _startFrameTime;
	unsigned int _startFrameQuery;

	float _lastFrameTime;

public:
	GPUProfilerService ();
	~GPUProfilerService ();

	void StartFrame ();

	const std::vector<ProfilerEntry*>& GetLastFrameEvents () const;

	uint64_t GetStartTime () const;
	float GetLastFrameTime () const;
};

#endif