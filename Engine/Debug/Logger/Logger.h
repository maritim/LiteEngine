#ifndef LOGGER_H
#define LOGGER_H

#include "Core/Singleton/Singleton.h"

#include <fstream>
#include <string>

#define DEBUG_LOG(message) Logger::Instance ()->Log (__FILE__, __LINE__, message)
#define DEBUG_LOGERROR(message) Logger::Instance ()->LogError (__FILE__, __LINE__, message)
#define DEBUG_LOGWARNING(message) Logger::Instance ()->LogWarning (__FILE__, __LINE__, message)

class Logger : public Singleton<Logger>
{
	friend Singleton<Logger>;

private:
	std::ofstream _outStream;

public:
	void Log (const char* file, int line, std::string message);
	void LogError (const char* file, int line, std::string message);
	void LogWarning (const char* file, int line, std::string message);

private:
	Logger ();
	~Logger ();
	Logger (const Logger&);
	Logger& operator=(const Logger&);
};

#endif