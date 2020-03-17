#include "Logger.h"

#include <spdlog/spdlog.h>

void Logger::Log (const char* filename, int line, const std::string& message)
{
	spdlog::debug (std::string () + filename + ":" + std::to_string (line) + ": " + message);
}

void Logger::LogError (const char* filename, int line, const std::string& message)
{
	spdlog::debug (std::string () + filename + ":" + std::to_string (line) + ": [Error]" + message);
}

void Logger::LogWarning (const char* filename, int line, const std::string& message)
{
	spdlog::debug (std::string () + filename + ":" + std::to_string (line) + ": [Warning]" + message);
}
