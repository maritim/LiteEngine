#include "TextureLUT.h"

TextureLUT::TextureLUT (const std::string& name) :
	Texture (name),
	_depth (0)
{
	_generateMipmaps = false;
}

void TextureLUT::SetDepth (std::size_t depth)
{
	_depth = depth;
}

std::size_t TextureLUT::GetDepth () const
{
	return _depth;
}
