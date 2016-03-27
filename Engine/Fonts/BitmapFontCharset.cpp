#include "BitmapFontCharset.h"

BitmapFontCharset::BitmapFontCharset ()
{

}

BitmapFontCharset::~BitmapFontCharset ()
{
	this->Clear ();
}

void BitmapFontCharset::AddChar (BitmapFontChar* ch)
{
	unsigned char value = ch->GetId ();
	this->RemoveChar (value);

	_hash [value] = ch;
}

void BitmapFontCharset::RemoveChar (unsigned char ch)
{
	auto item = _hash.find (ch);
	if (item != _hash.end ()) {
		delete item->second;
		_hash.erase (item);
	}
}

BitmapFontChar* BitmapFontCharset::GetChar (unsigned char ch)
{
	auto item = _hash.find (ch);
	if (item == _hash.end ()) {
		return nullptr;
	}

	return item->second;
}

void BitmapFontCharset::Clear ()
{
	for (auto item : _hash) {
		delete item.second;
	}

	_hash.clear ();	
}