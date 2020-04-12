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
