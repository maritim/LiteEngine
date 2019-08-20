#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include "Core/Interfaces/Object.h"

#include "SceneGraph/Transform.h"

#include "Core/Resources/Resource.h"
#include "AudioViews/AudioClipView.h"
#include "AudioClip.h"

class ENGINE_API AudioSource : public Object
{
protected:
	Transform* _transform;
	Resource<AudioClip> _audioClip;
	Resource<AudioClipView> _audioClipView;

	unsigned int _sourceID;

public:
	AudioSource (Transform* transform);

	void SetAudioClip (const Resource<AudioClip>& audioClip);
	void SetVolume (float volume);
	void SetLoop (bool loop);

	Resource<AudioClip> GetAudioClip () const;
	bool IsPlaying () const;

	void Update ();

	void Play ();
	void Stop ();
	void Pause ();
};

#endif