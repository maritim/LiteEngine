#ifndef BITMAPFONTCOMMON_H
#define BITMAPFONTCOMMON_H

#include "Core/Interfaces/Object.h"

class BitmapFontCommon : public Object
{
protected:
	// lineHeight	This is the distance in pixels between each line of text.
	std::size_t _lineHeight;
	// base	The number of pixels from the absolute top of the line to the 
	// base of the characters.
	std::size_t _base;
	// scaleW	The width of the texture, normally used to scale the x pos 
	// of the character image.
	std::size_t _scaleW;
	// scaleH	The height of the texture, normally used to scale the y pos 
	// of the character image.
	std::size_t _scaleH;
	// pages	The number of texture pages included in the font.
	std::size_t _pages;
	// packed	Set to 1 if the monochrome characters have been packed into each 
	// of the texture channels. In this case alphaChnl describes what is stored 
	// in each channel.
	char _packed;
	// redChnl	Set to 0 if the channel holds the glyph data, 1 if it holds the 
	// outline, 2 if it holds the glyph and the outline, 3 if its set to zero, 
	// and 4 if its set to one.
	char _alphaChannel;
	// redChnl	Set to 0 if the channel holds the glyph data, 1 if it holds the 
	// outline, 2 if it holds the glyph and the outline, 3 if its set to zero, 
	// and 4 if its set to one.
	char _redChannel;
	// greenChnl	Set to 0 if the channel holds the glyph data, 1 if it holds 
	// the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, 
	// and 4 if its set to one.
	char _greenChannel;
	// blueChnl	Set to 0 if the channel holds the glyph data, 1 if it holds 
	// the outline, 2 if it holds the glyph and the outline, 3 if its set 
	// to zero, and 4 if its set to one.
	char _blueChannel;

public:
	BitmapFontCommon ();
	~BitmapFontCommon ();

	std::size_t GetLineHeight () const;
	std::size_t GetBase () const;
	std::size_t GetWidthScale () const;
	std::size_t GetHeightScale () const;
	std::size_t GetPageCount () const;

	char IsPacked () const;
	char IsAlphaChannel () const;
	char IsRedChannel () const;
	char IsGreenChannel () const;
	char IsBlueChannel () const;

	void SetLineHeight (std::size_t lineHeight);
	void SetBase (std::size_t base);
	void SetWidthScale (std::size_t scaleW);
	void SetHeightScale (std::size_t scaleH);
	void SetPagesCount (std::size_t pages);

	void SetPacked (char packed);
	void SetAlphaChannel (char alphaChannel);
	void SetRedChannel (char redChannel);
	void SetGreenChannel (char greenChannel);
	void SetBlueChannel (char blueChannel);
};

#endif