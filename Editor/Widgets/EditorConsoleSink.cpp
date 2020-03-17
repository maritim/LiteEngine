#include "EditorConsoleSink.h"

const std::vector<ConsoleLog>& EditorConsoleSink::GetLog () const
{
	return _logs;
}

void EditorConsoleSink::ClearLog ()
{
	_logs.clear ();
}

void EditorConsoleSink::Sink (const ConsoleLog& message)
{
	_logs.push_back (message);
}

void EditorConsoleSink::Flush ()
{

}
