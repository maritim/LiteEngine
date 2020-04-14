#include "AudioSourceComponent.h"

AudioSourceComponent::AudioSourceComponent () :
	_audioSource (nullptr)
{

}

AudioSourceComponent::~AudioSourceComponent ()
{
	delete _audioSource;
}

void AudioSourceComponent::Awake ()
{
	_audioSource = new AudioSource (_parent->GetTransform ());

	_audioSource->SetAudioClip (_audioClip);
	_audioSource->SetVolume (_volume);
	_audioSource->SetLoop (_loop);
}

void AudioSourceComponent::Update ()
{
	_audioSource->Update ();
}

void AudioSourceComponent::SetAudioClip (const Resource<AudioClip>& audioClip)
{
	_audioClip = audioClip;

	_audioSource->SetAudioClip (audioClip);
}

void AudioSourceComponent::SetVolume (float volume)
{
	_audioSource->SetVolume (volume);
}

void AudioSourceComponent::SetLoop (bool loop)
{
	_audioSource->SetLoop (loop);
}
