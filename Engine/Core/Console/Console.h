#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>

#include "ConsoleSink.h"

class ENGINE_API Console
{
public:
	static void Init ();
	static void Quit ();

	static void Log (const std::string& message);
	static void LogError (const std::string& message);
	static void LogWarning (const std::string& message);

	static void AttachSink (ConsoleSink* consoleSink);
};

#endif