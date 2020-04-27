#ifndef AUDIOSOURCECOMPONENT_H
#define AUDIOSOURCECOMPONENT_H

#include "Systems/Components/Component.h"

#include "Audio/AudioSource.h"

class ENGINE_API AudioSourceComponent : public Component
{
	DECLARE_COMPONENT(AudioSourceComponent)

protected:
	ATTRIBUTE(EditAnywhere, Meta=(AudioClip))
	Resource<AudioClip> _audioClip;
	ATTRIBUTE(EditAnywhere, Meta)
	float _volume;
	ATTRIBUTE(EditAnywhere, Meta)
	bool _loop;

	AudioSource* _audioSource;

public:
	AudioSourceComponent ();
	~AudioSourceComponent ();

	void Awake ();

	void Update ();

	void SetAudioClip (const Resource<AudioClip>& audioClip);
	void SetVolume (float volume);
	void SetLoop (bool loop);
};

#endif