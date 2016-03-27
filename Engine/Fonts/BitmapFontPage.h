#ifndef BITMAPFONTPAGE_H
#define BITMAPFONTPAGE_H

#include "Core/Interfaces/Object.h"

#include "BitmapFontCharset.h"
#include "Texture/Texture.h"

class BitmapFontPage : public Object
{
private:
	std::size_t _id;
	BitmapFontCharset* _charset;
	Texture* _texture;

public:
	BitmapFontPage ();
	~BitmapFontPage ();

	std::size_t GetId () const;
	BitmapFontCharset* GetCharset () const;
	Texture* GetTexture () const;

	void SetId (std::size_t id);
	void SetCharset (BitmapFontCharset* charset);
	void SetTexture (Texture* texture);
};

#endif