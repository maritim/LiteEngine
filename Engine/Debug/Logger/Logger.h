#ifndef LOGGER_H
#define LOGGER_H

#include <string>

#define DEBUG_LOG(message) Logger::Log (__FILE__, __LINE__, message)
#define DEBUG_LOGERROR(message) Logger::LogError (__FILE__, __LINE__, message)
#define DEBUG_LOGWARNING(message) Logger::LogWarning (__FILE__, __LINE__, message)

class ENGINE_API Logger
{
public:
	static void Log (const char* file, int line, const std::string& message);
	static void LogError (const char* file, int line, const std::string& message);
	static void LogWarning (const char* file, int line, const std::string& message);
};

#endif