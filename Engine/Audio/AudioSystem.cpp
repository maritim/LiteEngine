#include "AudioSystem.h"

#include "Wrappers/OpenAL/AL.h"

Resource<AudioClipView> AudioSystem::LoadAudioClip (const Resource<AudioClip>& audioClip)
{
	AudioClipView* audioClipView = new AudioClipView ();

	ALuint buffer;

	AL::GenBuffers ((ALuint)1, &buffer);

	AL::BufferData (buffer, AL_FORMAT_STEREO16,
		audioClip->GetData (),
		audioClip->GetSize (),
		audioClip->GetSampleRate ());

	audioClipView->SetBufferID (buffer);

	return Resource<AudioClipView> (audioClipView, audioClip->GetName ());
}