#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <string>
#include <vector>

class Argument
{
protected:
	std::string _name;
	std::vector<std::string> _args;

public:
	Argument (const std::string& first, const std::string& second);
	Argument (const std::string&);

	const std::string& GetName ();
	const std::vector<std::string>& GetArgs ();
};

#endif