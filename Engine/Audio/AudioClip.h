#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H

#include "Core/Interfaces/Object.h"

class ENGINE_API AudioClip : public Object
{
protected:
	std::string _name;
	unsigned char* _data;
	std::size_t _size;
	std::size_t _sampleRate;

public:
	AudioClip ();
	~AudioClip ();

	void SetName (const std::string& name);
	void SetData (unsigned char* data, std::size_t size, std::size_t sampleRate);
	
	std::string GetName () const;
	unsigned char* GetData () const;
	std::size_t GetSize () const;
	std::size_t GetSampleRate () const;
	float GetDuration () const;
};

#endif