#ifndef TEXTUREVIEW_H
#define TEXTUREVIEW_H

#include "Core/Interfaces/Object.h"

class TextureView : public Object
{
protected:
	unsigned int _gpuIndex;

public:
	TextureView ();
	~TextureView ();

	virtual void Activate (std::size_t textureUnit);

	void SetGPUIndex (unsigned int gpuIndex);

	unsigned int GetGPUIndex () const;
};

#endif