#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include "Core/Resources/Resource.h"
#include "Audio/AudioClip.h"
#include "Audio/AudioViews/AudioClipView.h"

class AudioSystem
{
public:
	static Resource<AudioClipView> LoadAudioClip (const Resource<AudioClip>&);
};

#endif