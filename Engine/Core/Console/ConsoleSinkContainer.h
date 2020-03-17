#ifndef CONSOLESINKCONTAINER_H
#define CONSOLESINKCONTAINER_H

#include <spdlog/sinks/base_sink.h>

#include <vector>
#include <mutex>

#include "ConsoleSink.h"

class ConsoleSinkContainer : public spdlog::sinks::base_sink<std::mutex>
{
protected:
	std::vector<ConsoleSink*> _sinks;

public:
	void AttachSink (ConsoleSink* sink);
protected:
	void sink_it_(const spdlog::details::log_msg& msg);
	void flush_();
};

#endif