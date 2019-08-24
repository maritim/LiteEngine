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

class GPUProfilerService : public ProfilerService
{
protected:
	std::vector<ProfilerEntry*> _lastFrameQueue2;

	unsigned long int _lastStartFrameTime;
	unsigned int _lastStartFrameQuery;
	unsigned long int _startFrameTime;
	unsigned int _startFrameQuery;

	float _lastFrameTime;

public:
	GPUProfilerService ();
	~GPUProfilerService ();

	void StartFrame ();

	const std::vector<ProfilerEntry*>& GetLastFrameEvents () const;

	unsigned long int GetStartTime () const;
	float GetLastFrameTime () const;
};

#endif