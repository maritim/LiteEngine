#include "OpenALModule.h"

#include "Wrappers/OpenAL/AL.h"

#include "Core/Console/Console.h"

ALCcontext* OpenALModule::_context (nullptr);

void OpenALModule::Init ()
{
	alGetError();

	ALCdevice* device = AL::OpenDevice (nullptr);

	if (device == nullptr) {
		Console::LogError ("Unable to initialize an OpenAL device. Exit now.");
		exit (0);
	}

	_context = AL::CreateContext (device, nullptr);
	if (!AL::MakeContextCurrent (_context)) {
		Console::LogError ("Unable to initialize current context. Exit now.");
		exit (0);
	}
}

void OpenALModule::Quit ()
{
	ALCdevice* device = AL::GetContextsDevice (_context);
	AL::MakeContextCurrent (nullptr);
	AL::DestroyContext (_context);
	AL::CloseDevice (device);
}