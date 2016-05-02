#include "Argument.h"

Argument::Argument (const std::string& name, const std::string& arg) :
	_name (name)
{
	_args.push_back (arg);
}

Argument::Argument (const std::string& name) :
	_name (name)
{

}

const std::string& Argument::GetName ()
{
	return _name;
}

const std::vector<std::string>& Argument::GetArgs ()
{
	return _args;
}