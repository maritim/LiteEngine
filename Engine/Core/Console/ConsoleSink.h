#ifndef CONSOLESINK_H
#define CONSOLESINK_H

#include "Core/Interfaces/Object.h"

#include <chrono>

enum LogPriority
{
	LOG_DEBUG = 1,
	LOG_INFO = 2,
	LOG_WARNING = 3,
	LOG_ERROR = 4
};

struct ConsoleLog
{
	std::string Message;
	LogPriority Priority;
	std::string Channel;
	std::chrono::system_clock::time_point Time;
};

class ENGINE_API ConsoleSink : public Object
{
public:
	virtual void Sink (const ConsoleLog& message) = 0;
	virtual void Flush () = 0;
};

#endif