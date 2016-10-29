#ifndef PROFILER_H
#define PROFILER_H

#include "Core/Singleton/Singleton.h"

#include <string>
#include <vector>

#include "ProfilerLogger.h"
#include "ProfilerFrame.h"

/*
 * TODO: Move this to another part of the code.
*/

#define PROFILER_ARGUMENT_FLAG "profiler"

struct ProfilerLoggerContainer
{
	std::string Name;
	float Duration;
	std::size_t NestDepth;

	ProfilerLoggerContainer ();
};

class Profiler : public Singleton<Profiler>
{
	friend Singleton<Profiler>;

private:
	bool _isActive;
	std::vector<ProfilerLoggerContainer> _eventsQueue;

public:
	bool IsActive () const;

	void AddEventAt (const ProfilerLoggerContainer&, std::size_t);
	const std::vector<ProfilerLoggerContainer>& GetEvents () const;

	void Clear ();
private:
	Profiler ();
	~Profiler ();
	Profiler (const Profiler&);
	Profiler& operator=(const Profiler&);

	void Allocate (std::size_t);
};

#endif