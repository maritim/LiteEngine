#include <iostream>

#include <string>

#include "Console.h"

std::ofstream Console::_outStream("Console.log");

void Console::Log (std::string message)
{
	_outStream << message << std::endl;
}

void Console::LogError (std::string message)
{
	_outStream << "Error: " << message << std::endl;
}

void Console::LogWarning (std::string message)
{
	_outStream << "Warning: " << message << std::endl;
}