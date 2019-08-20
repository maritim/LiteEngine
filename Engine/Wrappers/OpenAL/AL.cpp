#include "AL.h"

#include "Core/Console/Console.h"

/*
 * Device
*/

ALCdevice* AL::OpenDevice (const ALCchar *devicename)
{
	auto result = alcOpenDevice (devicename);

	ErrorCheck ("alcOpenDevice");

	return result;
}

ALCboolean AL::CloseDevice(ALCdevice *device)
{
	auto result = alcCloseDevice (device);

	ErrorCheck ("alcCloseDevice");

	return result;
}

/*
 * Context
*/

ALCcontext* AL::CreateContext(ALCdevice *device, ALCint* attrlist)
{
	auto result = alcCreateContext (device, attrlist);

	ErrorCheck ("alcCreateContext");

	return result;
}

ALCboolean AL::MakeContextCurrent(ALCcontext *context)
{
	auto result = alcMakeContextCurrent (context);

	ErrorCheck ("alcMakeContextCurrent");

	return result;
}

ALCdevice* AL::GetContextsDevice(ALCcontext *context)
{
	auto result = alcGetContextsDevice (context);

	ErrorCheck ("alcGetContextsDevice");

	return result;
}

void AL::DestroyContext(ALCcontext *context)
{
	alcDestroyContext (context);

	ErrorCheck ("alcDestroyContext");
}

/*
 * Buffers
*/

void AL::GenBuffers(ALsizei n, ALuint *buffers)
{
	alGenBuffers (n, buffers);

	ErrorCheck ("alGenBuffers");
}

void AL::BufferData(ALuint buffer, ALenum format, const ALvoid *data, ALsizei size, ALsizei freq)
{
	alBufferData (buffer, format, data, size, freq);

	ErrorCheck ("alBufferData");
}

void AL::DeleteBuffers(ALsizei n, ALuint *buffers)
{
	alDeleteBuffers (n, buffers);

	ErrorCheck ("alDeleteBuffers");
}

/*
 * Sources
*/

void AL::GenSources(ALsizei n, ALuint *sources)
{
	alGenSources (n, sources);

	ErrorCheck ("alGenSources");
}

void AL::Sourcef(ALuint source, ALenum param, ALfloat value)
{
	alSourcef (source, param, value);

	ErrorCheck ("alSourcef");
}

void AL::Source3f(ALuint source, ALenum param, ALfloat v1, ALfloat v2, ALfloat v3)
{
	alSource3f (source, param, v1, v2, v3);

	ErrorCheck ("alSource3f");
}

void AL::Sourcei(ALuint source, ALenum param, ALint value)
{
	alSourcei (source, param, value);

	ErrorCheck ("alSourcei");
}

void AL::SourcePlay(ALuint source)
{
	alSourcePlay (source);

	ErrorCheck ("alSourcePlay");
}

void AL::SourcePause(ALuint source)
{
	alSourcePause (source);

	ErrorCheck ("alSourcePause");
}

void AL::SourceStop(ALuint source)
{
	alSourceStop (source);

	ErrorCheck ("alSourceStop");
}

void AL::GetSourcei(ALuint source, ALenum pname, ALint *value)
{
	alGetSourcei (source, pname, value);

	ErrorCheck ("alGetSourcei");
}

void AL::ErrorCheck (const std::string& methodName)
{
	ALCenum error;
	if ((error = alGetError()) != AL_NO_ERROR) {
		std::string errorString = "On " + methodName + ": ";

		switch (error) {
			case AL_INVALID_NAME:
				errorString += "AL_INVALID_NAME (Invalid name parameter.)";
				break;
			case AL_INVALID_ENUM:
				errorString += "AL_INVALID_ENUM (Invalid parameter.)";
				break;
			case AL_INVALID_VALUE:
				errorString += "AL_INVALID_VALUE (Invalid enum parameter value.)";
				break;
			case AL_INVALID_OPERATION:
				errorString += "AL_INVALID_OPERATION (Illegal call.)";
				break;
			case AL_OUT_OF_MEMORY:
				errorString += "AL_OUT_OF_MEMORY (Unable to allocate memory.)";
				break;
		}

		Console::LogError (errorString);
	}
}