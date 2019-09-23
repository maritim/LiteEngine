#include "WAVLoader.h"

#ifdef _WIN32
	#include <SDL2/SDL_audio.h>
#elif defined(__linux__)
	#include <SDL/SDL_audio.h>
#endif

#include "Audio/AudioClip.h"

#include "Core/Console/Console.h"

Object* WAVLoader::Load(const std::string& filename)
{
	AudioClip* audioClip = new AudioClip ();

	SDL_AudioSpec spec;
	unsigned int size;
	unsigned char* buffer;

	if (SDL_LoadWAV (filename.c_str (), &spec, &buffer, &size) == nullptr) {
		Console::LogError ("Unable to load \"" + filename + "\"!");
		exit (0);
	}

	audioClip->SetName (filename);
	audioClip->SetData (buffer, size, spec.freq);

	return audioClip;
}
