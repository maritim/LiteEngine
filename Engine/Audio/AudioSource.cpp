#include "AudioSource.h"

#include "AudioSystem.h"

#include "Wrappers/OpenAL/AL.h"

AudioSource::AudioSource (Transform* transform) :
	_transform (transform),
	_audioClip (nullptr),
	_audioClipView (nullptr),
	_sourceID (0)
{
	AL::GenSources((ALuint)1, &_sourceID);

	AL::Sourcef(_sourceID, AL_PITCH, 1);
	AL::Sourcef(_sourceID, AL_GAIN, 1);
	AL::Source3f(_sourceID, AL_POSITION, 0, 0, 0);
	AL::Source3f(_sourceID, AL_VELOCITY, 0, 0, 0);
	AL::Sourcei(_sourceID, AL_LOOPING, AL_FALSE);
}

void AudioSource::SetAudioClip (const Resource<AudioClip>& audioClip)
{
	_audioClip = audioClip;
	_audioClipView = AudioSystem::LoadAudioClip (audioClip);

	AL::Sourcei(_sourceID, AL_BUFFER, _audioClipView->GetBufferID ());
}

void AudioSource::SetVolume (float volume)
{
	AL::Sourcef(_sourceID, AL_GAIN, volume);
}

void AudioSource::SetLoop (bool loop)
{
	AL::Sourcei(_sourceID, AL_LOOPING, loop);
}

Resource<AudioClip> AudioSource::GetAudioClip () const
{
	return _audioClip;
}

bool AudioSource::IsPlaying () const
{
    ALenum state;
    
    AL::GetSourcei(_sourceID, AL_SOURCE_STATE, &state);
    
    return (state == AL_PLAYING);
}

void AudioSource::Update ()
{
	if (_audioClip == nullptr) {
		return;
	}

	glm::vec3 position = _transform->GetPosition ();

	AL::Source3f(_sourceID, AL_POSITION, position.x, position.y, position.z);
}

void AudioSource::Play ()
{
	AL::SourcePlay (_sourceID);
}

void AudioSource::Stop ()
{
	AL::SourceStop (_sourceID);
}

void AudioSource::Pause ()
{
	AL::SourcePause (_sourceID);
}