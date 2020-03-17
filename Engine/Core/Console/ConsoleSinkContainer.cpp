#include "ConsoleSinkContainer.h"

void ConsoleSinkContainer::AttachSink (ConsoleSink* sink)
{
	_sinks.push_back (sink);
}

void ConsoleSinkContainer::sink_it_(const spdlog::details::log_msg& msg)
{

	// log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
	// msg.raw contains pre formatted log

	// If needed (very likely but not mandatory), the sink formats the message before sending it to its final destination:
	spdlog::memory_buf_t formatted;
	base_sink<std::mutex>::formatter_->format(msg, formatted);
	
	ConsoleLog consoleLog;

	consoleLog.Message = fmt::to_string(formatted);
	consoleLog.Priority = (LogPriority) msg.level;
	consoleLog.Time = msg.time;

	for (auto sink : _sinks) {
		sink->Sink (consoleLog);
	}
}

void ConsoleSinkContainer::flush_()
{
	for (auto sink : _sinks) {
		sink->Flush ();
	}
}