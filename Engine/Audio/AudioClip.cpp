#include "AudioClip.h"

AudioClip::AudioClip () :
	_name (),
	_data (nullptr),
	_size (0),
	_sampleRate (0)
{

}

AudioClip::~AudioClip ()
{
	free (_data);
}

void AudioClip::SetName (const std::string& name)
{
	_name = name;
}

void AudioClip::SetData (unsigned char* data, std::size_t size, std::size_t sampleRate)
{
	_data = data;
	_size = size;
	_sampleRate = sampleRate;
}

std::string AudioClip::GetName () const
{
	return _name;
}

unsigned char* AudioClip::GetData () const
{
	return _data;
}

std::size_t AudioClip::GetSize () const
{
	return _size;
}

std::size_t AudioClip::GetSampleRate () const
{
	return _sampleRate;
}
