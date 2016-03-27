#ifndef BITMAPFONTINFO_H
#define BITMAPFONTINFO_H

#include "Core/Interfaces/Object.h"

#include <string>

#include "Core/Math/glm/glm.hpp"

class BitmapFontInfo : public Object
{
protected:
	// face	This is the name of the true type font.
	std::string _face;
	// size	The size of the true type font.
	std::size_t _size;
	// bold	The font is bold.
	bool _bold;
	// italic	The font is italic.
	bool _italic;
	// charset	The name of the OEM charset used (when not unicode).
	std::string _charset;
	// unicode	Set to 1 if it is the unicode charset.
	bool _unicode;
	// stretchH	The font height stretch in percentage. 100% means no stretch.
	float _stretchH;
	// smooth	Set to 1 if smoothing was turned on.
	bool _smooth;
	// aa	The supersampling level used. 1 means no supersampling was used.
	std::size_t _superSampling;
	// padding	The padding for each character (up, right, down, left).
	glm::uvec4 _padding;
	// spacing	The spacing for each character (horizontal, vertical).
	glm::uvec2 _spacing;
	// outline	The outline thickness for the characters.
	float _outline;

public:
	BitmapFontInfo ();
	virtual ~BitmapFontInfo ();

	std::string GetFace () const;
	std::size_t GetSize () const;
	std::string GetCharset () const;
	float GetHeightStretch () const;
	std::size_t GetSuperSampling () const;
	glm::uvec4 GetPadding () const;
	glm::uvec2 GetSpacing () const;
	float GetOutline () const;

	bool IsBold () const;
	bool IsItalic () const;
	bool IsUnicode () const;
	bool IsSmooth () const;

	void SetFace (const std::string& face);
	void SetSize (std::size_t size);
	void SetBold (bool bold);
	void SetItalic (bool italic);
	void SetCharset (const std::string& charset);
	void SetUnicode (bool unicode);
	void SetHeightStretch (float stretchH);
	void SetSmooth (bool smooth);
	void SetSuperSampling (std::size_t supersampling);
	void SetPadding (int x = 0, int y = 0, int z = 0, int w = 0);
	void SetPadding (const glm::uvec4& padding);
	void SetSpacing (int x = 0, int y = 0);
	void SetSpacing (const glm::uvec2& spacing);
	void SetOutline (float outline);
};

#endif