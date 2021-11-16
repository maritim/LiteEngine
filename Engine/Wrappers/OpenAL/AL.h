#ifndef AL_H
#define AL_H

#include <string>

#ifdef __APPLE__
#	include <OpenAL/al.h>
#	include <OpenAL/alc.h>
#else
#	include <AL/al.h>
#	include <AL/alc.h>
#endif

class AL
{
public:

	/*
	 * Device
	*/

	static ALCdevice* OpenDevice(const ALCchar *devicename);
	static ALCboolean CloseDevice(ALCdevice *device);

	/*
	 * Context
	*/

	static ALCcontext* CreateContext(ALCdevice *device, ALCint* attrlist);
	static ALCboolean MakeContextCurrent(ALCcontext *context);
	static ALCdevice* GetContextsDevice(ALCcontext *context);
	static void DestroyContext(ALCcontext *context);

	/*
	 * Buffers
	*/

	static void GenBuffers(ALsizei n, ALuint *buffers);
	static void BufferData(ALuint buffer, ALenum format, const ALvoid *data, ALsizei size, ALsizei freq);
	static void DeleteBuffers(ALsizei n, ALuint *buffers);

	/*
	 * Sources
	*/

	static void GenSources(ALsizei n, ALuint *sources);
	static void Sourcef(ALuint source, ALenum param, ALfloat value);
	static void Source3f(ALuint source, ALenum param, ALfloat v1, ALfloat v2, ALfloat v3);
	static void Sourcei(ALuint source, ALenum param, ALint value);
	static void SourcePlay(ALuint source);
	static void SourcePause(ALuint source);
	static void SourceStop(ALuint source);

	/*
	 * Getters
	*/

	static void GetSourcei(ALuint source, ALenum pname, ALint *value);

private:
	static void ErrorCheck (const std::string& methodName);
};

#endif
