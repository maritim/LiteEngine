#include "Console.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "ConsoleSinkContainer.h"

static std::shared_ptr<ConsoleSinkContainer> _sinkContainer (nullptr);

void Console::Init ()
{
	auto sink = std::make_shared<spdlog::sinks::dist_sink_st>();

	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Console.log", true);

	_sinkContainer = std::make_shared<ConsoleSinkContainer> ();

	sink->add_sink (file_sink);
	sink->add_sink (_sinkContainer);

	std::shared_ptr<spdlog::logger> logger = std::shared_ptr<spdlog::logger> (new spdlog::logger ("logger", sink));

	spdlog::set_default_logger (logger);

	spdlog::set_pattern ("[%H:%M:%S] [%l] %v");
	spdlog::set_level (spdlog::level::trace);
}

void Console::Quit ()
{

}

void Console::Log (const std::string& message)
{
	spdlog::info (message);
}

void Console::LogError (const std::string& message)
{
	spdlog::error (message);
}

void Console::LogWarning (const std::string& message)
{
	spdlog::warn (message);
}

void Console::AttachSink (ConsoleSink* consoleSink)
{
	_sinkContainer->AttachSink (consoleSink);
}
