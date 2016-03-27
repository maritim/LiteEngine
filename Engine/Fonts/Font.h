#ifndef FONT_H
#define FONT_H

#include "Core/Interfaces/Object.h"

#include "Texture/Texture.h"

#include "FontChar.h"

class Font : public Object
{
public:
	Font ();
	virtual ~Font ();

	virtual FontChar* GetChar (unsigned char id) = 0;
	virtual Texture* GetTexture (std::size_t id) = 0;
	virtual std::size_t GetLineHeight () = 0;
};

#endif