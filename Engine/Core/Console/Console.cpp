#include <iostream>

#include <string>

#include "Console.h"

void Console::Log (std::string message)
{
	std::cout << message << std::endl;
}

void Console::LogError (std::string message)
{
	std::cout << "Error: " << message << std::endl;
}

void Console::LogWarning (std::string message)
{
	std::cout << "Warning: " << message << std::endl;
}