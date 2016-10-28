#include "ArgumentsAnalyzer.h"

#include "Utils/Extensions/StringExtend.h"

ArgumentsAnalyzer::ArgumentsAnalyzer ()
{

}

ArgumentsAnalyzer::~ArgumentsAnalyzer ()
{
	Clear ();
}

void ArgumentsAnalyzer::ProcessArguments (int argc, char** argv)
{
	Clear ();

	std::string lastArgument = "";

	for (int i=1;i<argc;i++) {
		std::string current = argv [i];

		// Command
		if (current [0] == '-' && lastArgument != "") {
			AddArgument (lastArgument, "");
		}

		if (current [0] == '-') {
			lastArgument = current;
			continue;
		}

		if (lastArgument != "") {
			AddArgument (lastArgument, current);
			lastArgument = "";
			continue;
		}
	}

	if (lastArgument != "") {
		AddArgument (lastArgument, "");
	}
}

bool ArgumentsAnalyzer::HaveArgument (const std::string& argName)
{
	return _arguments.find (argName) != _arguments.end ();
}

Argument* ArgumentsAnalyzer::GetArgument (const std::string& argName)
{
	std::map<std::string, Argument*>::iterator it;
	std::string argName2 = argName;
	Extensions::StringExtend::Trim (argName2, '-');
	argName2 = "--" + argName2;

	it = _arguments.find (argName2);

	if (it == _arguments.end ()) {
		return nullptr;
	}

	return it->second;
}

void ArgumentsAnalyzer::AddArgument (const std::string& argName, const std::string& arg)
{
	Argument* argument = new Argument (argName, arg);

	_arguments [argName] = argument;
}

void ArgumentsAnalyzer::Clear ()
{
	std::map<std::string, Argument*>::iterator it;

	it = _arguments.begin ();

	for (; it != _arguments.end (); it++) {
		delete it->second;
	}

	_arguments.clear ();
}