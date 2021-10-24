#ifndef BITMAPFONTCHARSET_H
#define BITMAPFONTCHARSET_H

#include "Core/Interfaces/Object.h"

#include <map>

#include "BitmapFontChar.h"

class BitmapFontCharset : public Object
{
protected:
	std::map<unsigned char, BitmapFontChar*> _hash;

public:
	BitmapFontCharset ();
	~BitmapFontCharset ();

	void AddChar (BitmapFontChar* ch);
	void RemoveChar (unsigned char ch);

	BitmapFontChar* GetChar (unsigned char ch);

	void Clear ();
};

#endif