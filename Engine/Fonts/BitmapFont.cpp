#include "BitmapFont.h"

BitmapFont::BitmapFont () :
	_info (nullptr),
	_commonInfo (nullptr)
{

}

BitmapFont::~BitmapFont ()
{
	delete _info;
	delete _commonInfo;

	std::map<std::size_t, BitmapFontPage*>::iterator it;

	for (it = _pages.begin (); it != _pages.end (); it++) {
		delete it->second;
	}
}

FontChar* BitmapFont::GetChar (unsigned char ch) const
{
	std::map<std::size_t, BitmapFontPage*>::const_iterator it;

	for (it = _pages.begin ();it != _pages.end (); it++) {
		FontChar* fntCh = it->second->GetCharset ()->GetChar (ch);

		if (fntCh == nullptr) {
			continue;
		}

		return fntCh;
	}

	return nullptr;
}

Resource<Texture> BitmapFont::GetTexture (std::size_t id) const
{
	std::map<std::size_t, BitmapFontPage*>::const_iterator it;

	for (it = _pages.begin ();it != _pages.end (); it++) {
		if (it->second->GetId () == id) {
			return it->second->GetTexture ();
		}
	}

	return nullptr;
}

std::size_t BitmapFont::GetLineHeight ()
{
	return _commonInfo->GetLineHeight ();
}

BitmapFontInfo* BitmapFont::GetInfo () const
{
	return _info;
}

void BitmapFont::SetInfo (BitmapFontInfo* info)
{
	if (_info != nullptr) {
		delete _info;
	}

	_info = info;
}

BitmapFontCommon* BitmapFont::GetCommon () const
{
	return _commonInfo;
}

void BitmapFont::SetCommon (BitmapFontCommon* common)
{
	if (_commonInfo != nullptr) {
		delete _commonInfo;
	}

	_commonInfo = common;
}

BitmapFontPage* BitmapFont::GetPage (std::size_t id) const
{
	auto it = _pages.find (id);

	if (it == _pages.end ()) {
		return nullptr;
	}

	return it->second;
}

void BitmapFont::AddPage (BitmapFontPage* page)
{
	std::size_t id = page->GetId ();

	auto it = _pages.find (id);

	if (it != _pages.end ()) {
		delete it->second;
	}

	_pages [id] = page;
}