#ifndef ARGUMENTSANALYZER_H
#define ARGUMENTSANALYZER_H

#include "Core/Singleton/Singleton.h"

#include <map>

#include "Argument.h"

class ArgumentsAnalyzer : public Singleton<ArgumentsAnalyzer>
{
	friend Singleton<ArgumentsAnalyzer>;

private:
	std::map<std::string, Argument*> _arguments;

public:
	void ProcessArguments (int, char**);

	bool HaveArgument (const std::string&);
	Argument* GetArgument (const std::string&);
private:
	ArgumentsAnalyzer ();
	~ArgumentsAnalyzer ();
	ArgumentsAnalyzer (const ArgumentsAnalyzer&);
	ArgumentsAnalyzer& operator=(const ArgumentsAnalyzer&);

	void AddArgument (const std::string&, const std::string&);

	void Clear ();
};

#endif