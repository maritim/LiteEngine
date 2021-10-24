#ifndef BITMAPFONTCHAR_H
#define BITMAPFONTCHAR_H

#include "FontChar.h"

class BitmapFontChar : public FontChar
{
protected:
	// page	The texture page where the character image is found.
	std::size_t _page;
	// chnl	The texture channel where the character image is found (1 = blue, 2 = green, 4 = red, 8 = alpha, 15 = all channels).
	char _channel;

public:
	BitmapFontChar ();
	~BitmapFontChar ();

	std::size_t GetPage () const;
	char GetChannel () const;

	void SetPage (std::size_t page);
	void SetChannel (char channel);
};

#endif