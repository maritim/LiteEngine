#ifndef EDITORCONSOLESINK_H
#define EDITORCONSOLESINK_H

#include "Core/Console/ConsoleSink.h"

#include <vector>

class EditorConsoleSink : public ConsoleSink
{
protected:
	std::vector<ConsoleLog> _logs;

public:
	const std::vector<ConsoleLog>& GetLog () const;
	void ClearLog ();

	void Sink (const ConsoleLog& message);
	void Flush ();
};

#endif