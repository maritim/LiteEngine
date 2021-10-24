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

Resource<Texture> BitmapFontPage::GetTexture () const
{
	return _texture;
}

// Resource<TextureView> BitmapFontPage::GetTextureView () const
// {
// 	return _textureView;
// }

void BitmapFontPage::SetId (std::size_t id)
{
	_id = id;
}

void BitmapFontPage::SetCharset (BitmapFontCharset* charset)
{
	_charset = charset;
}

void BitmapFontPage::SetTexture (const Resource<Texture>& texture)
{
	_texture = texture;
}