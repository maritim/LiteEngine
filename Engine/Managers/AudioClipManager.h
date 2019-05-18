#ifndef AUDIOCLIPMANAGER_H
#define AUDIOCLIPMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <map>

#include "Audio/AudioClip.h"

class ENGINE_API AudioClipManager : public Singleton<AudioClipManager>
{
	friend Singleton<AudioClipManager>;

	DECLARE_SINGLETON(AudioClipManager)

private:
	std::map<std::string, AudioClip*> _audioClips;

public:
	void AddAudioClip (AudioClip* audioClip);
	AudioClip* GetAudioClip (const std::string& name);
private:
	void Load (AudioClip* audioClip);

	AudioClipManager ();
	~AudioClipManager ();
	AudioClipManager (const AudioClipManager&);
	AudioClipManager& operator=(const AudioClipManager&);
};

#endif