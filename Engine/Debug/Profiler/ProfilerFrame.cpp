#include "ProfilerFrame.h"

#include <fstream>
#include <string>
#include <vector>

#include "Debug/Profiler/Profiler.h"

std::size_t ProfilerFrame::_frameCount (0);

ProfilerFrame::ProfilerFrame ()
{
	/*
	 * TODO: Find a way to avoid usage of guards here.
	*/

	if (!Profiler::Instance ()->IsActive ()) {
		return;
	}

	_frameCount ++;

	Profiler::Instance ()->Clear ();
}

ProfilerFrame::~ProfilerFrame ()
{
	if (!Profiler::Instance ()->IsActive ()) {
		return;
	}

	LogFrame ();
}

void ProfilerFrame::LogFrame ()
{
	std::ofstream outStream;

	/*
	 * TODO: Refactor this
	*/

	if (_frameCount == 1) {
		outStream.open (PROFILER_OUTPUT_FILE, std::ofstream::out);
	} else {
		outStream.open (PROFILER_OUTPUT_FILE, std::ofstream::app);
	}

	/*
	 * TODO: Expand this
	*/

	outStream << "Log frame #" << _frameCount << std::endl;

	for (std::size_t i=0;i<Profiler::Instance ()->GetEvents ().size ();i++) {
		const ProfilerLoggerContainer& event = Profiler::Instance ()->GetEvents () [i];
		
		std::string logMessage;

		for (std::size_t nestDepth = event.NestDepth; nestDepth > 0; nestDepth --) {
			logMessage += "\t";
		}

		logMessage += "Event: " + event.Name;
		logMessage += " -> Duration: " + std::to_string (event.Duration) + " (ms)" + "\n";

		outStream << logMessage;
	}

	outStream << "End frame" << std::endl;

	outStream.close ();	
}