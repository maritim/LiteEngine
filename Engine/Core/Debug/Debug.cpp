#include "Debug.h"

#include <iostream>
#include <string>
#include <fstream>

Debug::Debug ()
{
	_outStream.open ("GameEngine.log");
}

Debug::~Debug ()
{
	_outStream.close ();
}

Debug& Debug::Instance ()
{
	static Debug debug;

	return debug;
}

void Debug::Log (const char* filename, int line, std::string message)
{
	_outStream << filename << ":" << line << ": " << message << std::endl;
}

void Debug::LogError (const char* filename, int line, std::string message)
{
	_outStream << filename << ":" << line << ": error: " << message << std::endl;
}

void Debug::LogWarning (const char* filename, int line, std::string message)
{
	_outStream << filename << ":" << line << ": warning: " << message << std::endl;
}

void Debug::RedirectToFile (std::string filename, bool append)
{
	if (append) {
		_outStream.open (filename.c_str (), std::fstream::app);
		return;
	}

	_outStream.open (filename.c_str(), std::fstream::out);
}

// TODO: implement this
void Debug::ChangeOutputStream (std::fstream outstream)
{
//	_outStream = outstream;
}