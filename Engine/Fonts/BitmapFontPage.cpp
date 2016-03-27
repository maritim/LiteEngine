#include "BitmapFontPage.h"

BitmapFontPage::BitmapFontPage () :
	_id (0),
	_charset (nullptr)
{

}

BitmapFontPage::~BitmapFontPage ()
{
	delete _charset;
}

std::size_t BitmapFontPage::GetId () const
{
	return _id;
}

BitmapFontCharset* BitmapFontPage::GetCharset () const
{
	return _charset;
}

Texture* BitmapFontPage::GetTexture () const
{
	return _texture;
}

void BitmapFontPage::SetId (std::size_t id)
{
	_id = id;
}

void BitmapFontPage::SetCharset (BitmapFontCharset* charset)
{
	_charset = charset;
}

void BitmapFontPage::SetTexture (Texture* texture)
{
	_texture = texture;
}