#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>

// TODO: Make this to redirect to file

class Console
{
public:
	static void Log (std::string message);
	static void LogError (std::string message);
	static void LogWarning (std::string message);
};

#endif