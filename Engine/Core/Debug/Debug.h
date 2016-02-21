#ifndef DEBUG_H
#define DEBUG_H

#include <fstream>
#include <string>

#define DEBUG_LOG(message) Debug::Instance ().Log (__FILE__, __LINE__, message)
#define DEBUG_LOGERROR(message) Debug::Instance ().LogError (__FILE__, __LINE__, message)
#define DEBUG_LOGWARNING(message) Debug::Instance ().LogWarning (__FILE__, __LINE__, message)

class Debug
{
private:
	std::fstream _outStream;

public:
	~Debug ();

	static Debug& Instance ();

	void Log (const char* file, int line, std::string message);
	void LogError (const char* file, int line, std::string message);
	void LogWarning (const char* file, int line, std::string message);

	void ChangeOutputStream (std::fstream outStream);
	void RedirectToFile (std::string filename, bool append = false);
private:
	Debug ();
};

#endif