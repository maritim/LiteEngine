#ifndef AUDIOCLIPVIEW_H
#define AUDIOCLIPVIEW_H

#include "Core/Interfaces/Object.h"

class AudioClipView : public Object
{
protected:
	unsigned int _bufferID;
public:
	AudioClipView ();
	~AudioClipView ();

	void SetBufferID (unsigned int bufferID);
	unsigned int GetBufferID () const;
};

#endif