/*
 * Bitmap Font implementation. Is Implemented
 * comformed with this documentation:
 * http://www.angelcode.com/products/bmfont/doc/file_format.html
 *
 * Notes. This implementation supports only ONE page of
 * charset for the font.
 */

#ifndef BITMAPFONT_H
#define BITMAPFONT_H

#include "Font.h"

#include <string>
#include <map>

#include "BitmapFontInfo.h"
#include "BitmapFontCommon.h"
#include "BitmapFontPage.h"

class BitmapFont : public Font
{
protected:
	BitmapFontInfo* _info;
	BitmapFontCommon* _commonInfo;
	std::map<std::size_t, BitmapFontPage*> _pages;

public:
	BitmapFont ();
	~BitmapFont ();

	virtual FontChar* GetChar (unsigned char ch);
	virtual Texture* GetTexture (std::size_t id);
	virtual std::size_t GetLineHeight ();

	std::string GetPageName () const;
	BitmapFontInfo* GetInfo () const;
	BitmapFontCommon* GetCommon () const;
	BitmapFontPage* GetPage (std::size_t id) const;

	void SetPageName () const;
	void SetInfo (BitmapFontInfo* info);
	void SetCommon (BitmapFontCommon* common);
	void AddPage (BitmapFontPage* page);
};

#endif