#include "FontChar.h"

FontChar::FontChar ()
{

}

FontChar::~FontChar ()
{
	
}

unsigned char FontChar::GetId () const
{
	return _id;
}

void FontChar::SetId (unsigned char id)
{
	_id = id;
}

glm::uvec2 FontChar::GetPosition () const
{
	return _position;
}

void FontChar::SetPosition (const glm::uvec2& position)
{
	_position = position;
}

glm::uvec2 FontChar::GetSize () const
{
	return _size;
}

void FontChar::SetSize (const glm::uvec2& size)
{
	_size = size;
}

glm::ivec2 FontChar::GetOffset () const
{
	return _offset;
}

void FontChar::SetOffset (const glm::ivec2& offset)
{
	_offset = offset;
}

short int FontChar::GetXAdvance () const
{
	return _xAdvance;
}

void FontChar::SetXAdvance (short int xAdvance)
{
	_xAdvance = xAdvance;
}

