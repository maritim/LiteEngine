#include "AudioClipView.h"

#include "Wrappers/OpenAL/AL.h"

AudioClipView::AudioClipView () :
	_bufferID (0)
{

}

AudioClipView::~AudioClipView ()
{
	AL::DeleteBuffers (1, &_bufferID);
}

void AudioClipView::SetBufferID (unsigned int bufferID)
{
	_bufferID = bufferID;
}

unsigned int AudioClipView::GetBufferID () const
{
	return _bufferID;
}
