#include "BitmapFontInfo.h"

BitmapFontInfo::BitmapFontInfo () :
	_face (),
	_size (0),
	_bold (false),
	_italic (false),
	_charset (),
	_unicode (false),
	_stretchH (100.0f),
	_smooth (false),
	_superSampling (1),
	_padding (0),
	_spacing (0),
	_outline (0)
{

}

BitmapFontInfo::~BitmapFontInfo ()
{

}

// face	This is the name of the true type font.
std::string BitmapFontInfo::GetFace () const
{
	return _face;
}


void BitmapFontInfo::SetFace (const std::string& face)
{
	_face = face;
}

// size	The size of the true type font.
std::size_t BitmapFontInfo::GetSize () const
{
	return _size;
}

void BitmapFontInfo::SetSize (std::size_t size)
{
	_size = size;
}

// bold	The font is bold.
bool BitmapFontInfo::IsBold () const
{
	return _bold;
}

void BitmapFontInfo::SetBold (bool bold)
{
	_bold = bold;
}

// italic	The font is italic.
bool BitmapFontInfo::IsItalic () const
{
	return _italic;
}

void BitmapFontInfo::SetItalic (bool italic)
{
	_italic = italic;
}

// charset	The name of the OEM charset used (when not unicode).
std::string BitmapFontInfo::GetCharset () const
{
	return _charset;
}

void BitmapFontInfo::SetCharset (const std::string& charset)
{
	_charset = charset;
}

// unicode	Set to 1 if it is the unicode charset.
bool BitmapFontInfo::IsUnicode () const
{
	return _unicode;
}

void BitmapFontInfo::SetUnicode (bool unicode)
{
	_unicode = unicode;
}

// stretchH	The font height stretch in percentage. 100% means no stretch.
float BitmapFontInfo::GetHeightStretch () const
{
	return _stretchH;
}

void BitmapFontInfo::SetHeightStretch (float stretchH)
{
	_stretchH = stretchH;
}

// smooth	Set to 1 if smoothing was turned on.
bool BitmapFontInfo::IsSmooth () const
{
	return _smooth;
}

void BitmapFontInfo::SetSmooth (bool smooth)
{
	_smooth = smooth;
}

// aa	The supersampling level used. 1 means no supersampling was used.
std::size_t BitmapFontInfo::GetSuperSampling () const
{
	return _superSampling;
}

void BitmapFontInfo::SetSuperSampling (std::size_t supersampling)
{
	_superSampling = supersampling;
}

// padding	The padding for each character (up, right, down, left).
glm::uvec4 BitmapFontInfo::GetPadding () const
{
	return _padding;
}

void BitmapFontInfo::SetPadding (const glm::uvec4& padding)
{
	_padding = padding;
}

void BitmapFontInfo::SetPadding (int x, int y, int z, int a)
{
	_padding = glm::uvec4 (x, y, z, a);
}

// spacing	The spacing for each character (horizontal, vertical).
glm::uvec2 BitmapFontInfo::GetSpacing () const
{
	return _spacing;
}

void BitmapFontInfo::SetSpacing (const glm::uvec2& spacing)
{
	_spacing = spacing;
}

void BitmapFontInfo::SetSpacing (int x, int y)
{
	_spacing = glm::uvec2 (x, y);
}

// outline	The outline thickness for the characters.
float BitmapFontInfo::GetOutline () const
{
	return _outline;
}

void BitmapFontInfo::SetOutline (float outline)
{
	_outline = outline;
}