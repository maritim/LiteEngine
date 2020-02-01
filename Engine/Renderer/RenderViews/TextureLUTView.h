#ifndef TEXTURELUTVIEW_H
#define TEXTURELUTVIEW_H

#include "TextureView.h"

class ENGINE_API TextureLUTView : public TextureView
{
public:
	void Activate (std::size_t textureUnit);
};

#endif