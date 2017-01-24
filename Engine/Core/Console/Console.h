#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>
#include <fstream>

// TODO: Make this to redirect to file

class Console
{
private:
	static std::ofstream _outStream;

public:
	static void Log (std::string message);
	static void LogError (std::string message);
	static void LogWarning (std::string message);
};

#endif