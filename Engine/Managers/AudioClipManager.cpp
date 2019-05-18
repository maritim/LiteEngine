#include "AudioClipManager.h"

#include "Wrappers/OpenAL/AL.h"

AudioClipManager::AudioClipManager () :
	_audioClips ()
{

}

AudioClipManager::~AudioClipManager ()
{
	std::map<std::string, AudioClip*>::iterator it;
	for (it = _audioClips.begin(); it != _audioClips.end(); ++it) {
		delete it->second;
	}
}

SPECIALIZE_SINGLETON(AudioClipManager)

void AudioClipManager::AddAudioClip (AudioClip* audioClip)
{
	if (audioClip == nullptr) {
		return ;
	}

	if (_audioClips.find (audioClip->GetName ()) != _audioClips.end ()) {
		return ;
	}

	Load (audioClip);

	_audioClips [audioClip->GetName ()] = audioClip;
}

AudioClip* AudioClipManager::GetAudioClip (const std::string& name)
{
	if (_audioClips.find (name) == _audioClips.end ()) {
		return nullptr;
	}

	return _audioClips [name];
}

void AudioClipManager::Load (AudioClip* audioClip)
{
	ALuint buffer;

	AL::GenBuffers ((ALuint)1, &buffer);

	AL::BufferData (buffer, AL_FORMAT_STEREO16,
		audioClip->GetData (),
		audioClip->GetSize (),
		audioClip->GetSampleRate ());

	audioClip->SetBufferID (buffer);
}
