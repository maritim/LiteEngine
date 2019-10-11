#ifndef PROFILERSERVICE_H
#define PROFILERSERVICE_H

#include "Core/Interfaces/Object.h"

#include <vector>

struct ProfilerEntry : public Object
{
	std::string Name;
	float Duration;
	float StartTime;
	std::size_t NestDepth;

	ProfilerEntry ();
};

class ENGINE_API ProfilerService
{
protected:
	bool _isActive;
	bool _nextActive;
	std::vector<ProfilerEntry*> _lastFrameQueue;
	std::vector<ProfilerEntry*> _currentFrameQueue;

public:
	ProfilerService ();

	void SetActive (bool isActive);
	bool IsActive () const;

	virtual void StartFrame ();

	void PushEvent (const ProfilerEntry*);
	virtual const std::vector<ProfilerEntry*>& GetLastFrameEvents () const;
	virtual const std::vector<ProfilerEntry*>& GetCurrentFrameEvents () const;

	void Clear ();
};

#endif