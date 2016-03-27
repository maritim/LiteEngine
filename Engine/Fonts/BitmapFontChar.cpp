#include "BitmapFontChar.h"

BitmapFontChar::BitmapFontChar () :
	_page (0),
	_channel (0)
{
	_position = glm::uvec2(0, 0);
	_size = glm::uvec2 (0, 0);
	_offset = glm::ivec2 (0, 0);
	_xAdvance = 0;
}

BitmapFontChar::~BitmapFontChar ()
{

}

std::size_t BitmapFontChar::GetPage () const
{
	return _page;
}

void BitmapFontChar::SetPage (std::size_t page)
{
	_page = page;
}

char BitmapFontChar::GetChannel () const
{
	return _channel;
}

void BitmapFontChar::SetChannel (char channel)
{
	_channel = channel;
}

