#include "Logger.h"

Logger::Logger () :
	_outStream ("GameEngine.log")
{

}

Logger::~Logger ()
{
	_outStream.close ();
}

SPECIALIZE_SINGLETON(Logger)

void Logger::Log (const char* filename, int line, std::string message)
{
	_outStream << filename << ":" << line << ": " << message << std::endl;
}

void Logger::LogError (const char* filename, int line, std::string message)
{
	_outStream << filename << ":" << line << ": [Error]: " << message << std::endl;
}

void Logger::LogWarning (const char* filename, int line, std::string message)
{
	_outStream << filename << ":" << line << ": [Warning]: " << message << std::endl;
}
