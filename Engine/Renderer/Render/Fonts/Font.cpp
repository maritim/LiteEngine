#include "Font.h"

Font::Font ()
{

}

Font::~Font ()
{
	
}

const std::string& Font::GetName () const
{
	return _name;
}

void Font::SetName (const std::string& name)
{
	_name = name;
}
