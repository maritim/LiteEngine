#include "BitmapFontCommon.h"

BitmapFontCommon::BitmapFontCommon () :
	_lineHeight (0),
	_base (0),
	_scaleW (0),
	_scaleH (0),
	_pages (1),
	_packed (false),
	_alphaChannel (true),
	_redChannel (true),
	_greenChannel (true),
	_blueChannel (true)
{

}

BitmapFontCommon::~BitmapFontCommon ()
{

}

// lineHeight	This is the distance in pixels between each line of text.
std::size_t BitmapFontCommon::GetLineHeight () const
{
	return _lineHeight;
}

void BitmapFontCommon::SetLineHeight (std::size_t lineHeight)
{
	_lineHeight = lineHeight;
}

// base	The number of pixels from the absolute top of the line to the 
// base of the characters.
std::size_t BitmapFontCommon::GetBase () const
{
	return _base;
}

void BitmapFontCommon::SetBase (std::size_t base)
{
	_base = base;
}

// scaleW	The width of the texture, normally used to scale the x pos 
// of the character image.
std::size_t BitmapFontCommon::GetWidthScale () const
{
	return _scaleW;
}

void BitmapFontCommon::SetWidthScale (std::size_t scaleW)
{
	_scaleW = scaleW;
}

// scaleH	The height of the texture, normally used to scale the y pos 
// of the character image.
std::size_t BitmapFontCommon::GetHeightScale () const
{
	return _scaleH;
}

void BitmapFontCommon::SetHeightScale (std::size_t scaleH)
{
	_scaleH = scaleH;
}

// pages	The number of texture pages included in the font.
std::size_t BitmapFontCommon::GetPageCount () const
{
	return _pages;
}

void BitmapFontCommon::SetPagesCount (std::size_t pages)
{
	_pages = pages;
}

// packed	Set to 1 if the monochrome characters have been packed into each 
// of the texture channels. In this case alphaChnl describes what is stored 
// in each channel.
char BitmapFontCommon::IsPacked () const
{
	return _packed;
}

void BitmapFontCommon::SetPacked (char packed)
{
	_packed = packed;
}

// alphaChnl	Set to 0 if the channel holds the glyph data, 1 if it holds 
// the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, 
// and 4 if its set to one.
char BitmapFontCommon::IsAlphaChannel () const
{
	return _alphaChannel;
}

void BitmapFontCommon::SetAlphaChannel (char alphaChannel)
{
	_alphaChannel = alphaChannel;
}

// redChnl	Set to 0 if the channel holds the glyph data, 1 if it holds the 
// outline, 2 if it holds the glyph and the outline, 3 if its set to zero, 
// and 4 if its set to one.
char BitmapFontCommon::IsRedChannel () const
{
	return _redChannel;
}

void BitmapFontCommon::SetRedChannel (char redChannel)
{
	_redChannel = redChannel;
}

// greenChnl	Set to 0 if the channel holds the glyph data, 1 if it holds 
// the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, 
// and 4 if its set to one.
char BitmapFontCommon::IsGreenChannel () const
{
	return _greenChannel;
}

void BitmapFontCommon::SetGreenChannel (char greenChannel)
{
	_greenChannel = greenChannel;
}

// blueChnl	Set to 0 if the channel holds the glyph data, 1 if it holds 
// the outline, 2 if it holds the glyph and the outline, 3 if its set 
// to zero, and 4 if its set to one.
char BitmapFontCommon::IsBlueChannel () const
{
	return _blueChannel;
}

void BitmapFontCommon::SetBlueChannel (char blueChannel)
{
	_blueChannel = blueChannel;
}
