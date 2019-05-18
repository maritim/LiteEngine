#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include "Core/Interfaces/Object.h"

#include "SceneGraph/Transform.h"

#include "AudioClip.h"

class ENGINE_API AudioSource : public Object
{
protected:
	Transform* _transform;
	AudioClip* _audioClip;

	unsigned int _sourceID;

public:
	AudioSource (Transform* transform);

	void SetAudioClip (AudioClip* audioClip);
	void SetVolume (float volume);
	void SetLoop (bool loop);

	AudioClip* GetAudioClip () const;
	bool IsPlaying () const;

	void Update ();

	void Play ();
	void Stop ();
	void Pause ();
};

#endif