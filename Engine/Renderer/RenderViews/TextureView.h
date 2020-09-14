#ifndef TEXTUREVIEW_H
#define TEXTUREVIEW_H

#include "Core/Interfaces/Object.h"

class ENGINE_API TextureView : public Object
{
protected:
	unsigned int _gpuIndex;
	int _textureType;

public:
	TextureView ();
	~TextureView ();

	virtual void Activate (std::size_t textureUnit);

	void SetGPUIndex (unsigned int gpuIndex);
	void SetType (int textureType);

	unsigned int GetGPUIndex () const;
};

#endif