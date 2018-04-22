#include "BitmapFontLoader.h"

#include <glm/glm.hpp>
#include <iostream>
#include <cctype>

#include "Utils/Extensions/StringExtend.h"
#include "Utils/Files/FileSystem.h"

#include "Resources.h"

#include "Managers/TextureManager.h"

Object* BitmapFontLoader::Load (const std::string& filename)
{
	std::ifstream f(filename);

	BitmapFont* font = new BitmapFont ();

	std::string data;
	while (f >> data)
	{
		if (data == "info") {
			ProcessBitmapFontInfo (f, font);
		}
		else if (data == "common") {
			ProcessBitmapFontCommon (f, font);
		}
		else if (data == "page") {
			ProcessBitmapFontPage (f, font, filename);
		}
		else if (data == "char") {
			ProcessBitmapFontChar (f, font);
		}
		else {
			ProcessComment (f);
		}
	}

	f.close ();

	return font;
}

void BitmapFontLoader::ProcessComment (std::ifstream& file)
{
	file.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
}

void BitmapFontLoader::ProcessBitmapFontInfo (std::ifstream& file, BitmapFont* font)
{
	std::string line;
	BitmapFontInfo* info = new BitmapFontInfo ();

	std::getline (file, line);

	for (std::size_t pos = 0; pos < line.size ();) {
		if (std::isspace (line [pos])) {
			++ pos;
			continue ;
		}

		std::pair<std::string, std::string> attr = ProcessAttribute (line, pos);

		if (attr.first == "face") {
			ProcessFace (attr.second, info);
		}
		else if (attr.first == "size") {
			ProcessSize (attr.second, info);
		}
		else if (attr.first == "charset") {
			ProcessCharset (attr.second, info);
		}
		else if (attr.first == "stretchH") {
			ProcessHeightStretch (attr.second, info);
		}
		else if (attr.first == "aa") {
			ProcessSuperSampling (attr.second, info);
		}
		else if (attr.first == "padding") {
			ProcessPadding (attr.second, info);
		}
		else if (attr.first == "spacing") {
			ProcessSpacing (attr.second, info);
		}
		else if (attr.first == "outline") {
			ProcessOutline (attr.second, info);
		}
		else if (attr.first == "bold") {
			ProcessBold (attr.second, info);
		}
		else if (attr.first == "italic") {
			ProcessItalic (attr.second, info);
		}
		else if (attr.first == "unicode") {
			ProcessUnicode (attr.second, info);
		}
		else if (attr.first == "smooth") {
			ProcessSmooth (attr.second, info);
		}
	}

	font->SetInfo (info);
}

void BitmapFontLoader::ProcessBitmapFontCommon (std::ifstream& file, BitmapFont* font)
{
	std::string line;
	BitmapFontCommon* common = new BitmapFontCommon ();

	std::getline (file, line);

	for (std::size_t pos = 0; pos < line.size ();) {
		if (std::isspace (line [pos])) {
			++ pos;
			continue;
		}

		std::pair<std::string, std::string> attr = ProcessAttribute (line, pos);

		if (attr.first == "lineHeight") {
			ProcessLineHeight (attr.second, common);
		}
		else if (attr.first == "base") {
			ProcessBase (attr.second, common);
		}
		else if (attr.first == "scaleW") {
			ProcessScaleWidth (attr.second, common);
		}
		else if (attr.first == "scaleH") {
			ProcessScaleHeight (attr.second, common);
		}
		else if (attr.first == "pages") {
			ProcessPages (attr.second, common);
		}
		else if (attr.first == "packed") {
			ProcessPacked (attr.second, common);
		}
		else if (attr.first == "alphaChnl") {
			ProcessAlphaChannel (attr.second, common);
		}
		else if (attr.first == "redChnl") {
			ProcessRedChannel (attr.second, common);
		}
		else if (attr.first == "greenChnl") {
			ProcessGreenChannel (attr.second, common);
		}
		else if (attr.first == "blueChnl") {
			ProcessBlueChannel (attr.second, common);
		}
	}

	font->SetCommon (common);
}

void BitmapFontLoader::ProcessBitmapFontPage (std::ifstream& file, BitmapFont* font,
	const std::string& filename)
{
	std::string line;
	BitmapFontPage* page = new BitmapFontPage ();

	std::getline (file, line);
	
	for (std::size_t pos = 0; pos < line.size ();) {
		if (std::isspace (line [pos])) {
			++ pos;
			continue;
		}

		std::pair<std::string, std::string> attr = ProcessAttribute (line, pos);

		if (attr.first == "id") {
			ProcessPageId (attr.second, page);
		}
		else if (attr.first == "file") {
			ProcessPageTexture (attr.second, page, filename);
		}
	}

	page->SetCharset (new BitmapFontCharset ());

	font->AddPage (page);
}

void BitmapFontLoader::ProcessBitmapFontChar (std::ifstream& file, BitmapFont* font)
{
	std::string line;
	BitmapFontChar* ch = new BitmapFontChar ();

	std::getline (file, line);

	for (std::size_t pos = 0; pos < line.size ();) {
		if (std::isspace (line [pos])) {
			++ pos;
			continue;
		}

		auto attr = ProcessAttribute (line, pos);

		if (attr.first == "id") {
			ProcessCharId (attr.second, ch);
		}
		else if (attr.first == "x") {
			ProcessCharPositionX (attr.second, ch);
		}
		else if (attr.first == "y") {
			ProcessCharPositionY (attr.second, ch);
		}
		else if (attr.first == "width") {
			ProcessCharSizeWidth (attr.second, ch);
		}
		else if (attr.first == "height") {
			ProcessCharSizeHeight (attr.second, ch);
		}
		else if (attr.first == "xoffset") {
			ProcessCharOffsetX (attr.second, ch);
		}
		else if (attr.first == "yoffset") {
			ProcessCharOffsetY (attr.second, ch);
		}
		else if (attr.first == "xadvance") {
			ProcessCharXAdvance (attr.second, ch);
		}
		else if (attr.first == "page") {
			ProcessCharPage (attr.second, ch);
		}
		else if (attr.first == "chnl") {
			ProcessCharChannel (attr.second, ch);
		}
	}

	font->GetPage (ch->GetPage ())->GetCharset ()->AddChar (ch);
}

void BitmapFontLoader::ProcessFace (const std::string& data, BitmapFontInfo* info)
{
	std::string faceName = data;
	Extensions::StringExtend::Trim (faceName, '"');

	info->SetFace (faceName);
}

void BitmapFontLoader::ProcessSize (const std::string& data, BitmapFontInfo* info)
{
	info->SetSize (std::stoul (data));
}

void BitmapFontLoader::ProcessCharset (const std::string& data, BitmapFontInfo* info)
{
	std::string charsetName = data;
	Extensions::StringExtend::Trim (charsetName, '"');

	info->SetCharset (charsetName);
}

void BitmapFontLoader::ProcessHeightStretch (const std::string& data, BitmapFontInfo* info)
{
	info->SetHeightStretch (std::stof (data));
}

void BitmapFontLoader::ProcessSuperSampling (const std::string& data, BitmapFontInfo* info)
{
	info->SetSuperSampling (std::stoul (data));
}

void BitmapFontLoader::ProcessPadding (const std::string& data, BitmapFontInfo* info)
{
	std::string charset = ",";
	std::vector<std::string> split = Extensions::StringExtend::Split (data, charset);
	glm::uvec4 padding;

	for (std::size_t i=0;i<split.size ();i++) {
		padding [i] = std::stoul (split [i]);
	}

	info->SetPadding (padding);
}

void BitmapFontLoader::ProcessSpacing (const std::string& data, BitmapFontInfo* info)
{
	std::string charset = ",";
	std::vector<std::string> split = Extensions::StringExtend::Split (data, charset);
	glm::uvec2 spacing;

	for (std::size_t i=0;i<split.size ();i++) {
		spacing [i] = std::stoul (split [i]);
	}

	info->SetSpacing (spacing);
}

void BitmapFontLoader::ProcessOutline (const std::string& data, BitmapFontInfo* info)
{
	info->SetOutline (std::stof (data));
}

void BitmapFontLoader::ProcessBold (const std::string& data, BitmapFontInfo* info)
{
	bool isBold = Extensions::StringExtend::ToBoolExt (data);

	info->SetBold (isBold);
}

void BitmapFontLoader::ProcessItalic (const std::string& data, BitmapFontInfo* info)
{
	bool isItalic = Extensions::StringExtend::ToBoolExt (data);

	info->SetItalic (isItalic);
}

void BitmapFontLoader::ProcessUnicode (const std::string& data, BitmapFontInfo* info)
{
	bool isUnicode = Extensions::StringExtend::ToBoolExt (data);

	info->SetUnicode (isUnicode);
}

void BitmapFontLoader::ProcessSmooth (const std::string& data, BitmapFontInfo* info)
{
	bool isSmooth = Extensions::StringExtend::ToBoolExt (data);

	info->SetSmooth (isSmooth);
}

void BitmapFontLoader::ProcessLineHeight (const std::string& data, BitmapFontCommon* common)
{
	common->SetLineHeight (std::stoul (data));
}

void BitmapFontLoader::ProcessBase (const std::string& data, BitmapFontCommon* common)
{
	common->SetBase (std::stoul (data));
}

void BitmapFontLoader::ProcessScaleWidth (const std::string& data, BitmapFontCommon* common)
{
	common->SetWidthScale (std::stoul (data));
}

void BitmapFontLoader::ProcessScaleHeight (const std::string& data, BitmapFontCommon* common)
{
	common->SetHeightScale (std::stoul (data));
}

void BitmapFontLoader::ProcessPages (const std::string& data, BitmapFontCommon* common)
{
	common->SetPagesCount (std::stoul (data));
}

void BitmapFontLoader::ProcessPacked (const std::string& data, BitmapFontCommon* common)
{
	common->SetPacked (std::stoi (data));
}

void BitmapFontLoader::ProcessAlphaChannel (const std::string& data, BitmapFontCommon* common)
{
	common->SetAlphaChannel (std::stoi (data));
}

void BitmapFontLoader::ProcessRedChannel (const std::string& data, BitmapFontCommon* common)
{
	common->SetRedChannel (std::stoi (data));
}

void BitmapFontLoader::ProcessGreenChannel (const std::string& data, BitmapFontCommon* common)
{
	common->SetGreenChannel (std::stoi (data));
}

void BitmapFontLoader::ProcessBlueChannel (const std::string& data, BitmapFontCommon* common)
{
	common->SetBlueChannel (std::stoi (data));
}

void BitmapFontLoader::ProcessPageId (const std::string& data, BitmapFontPage* page)
{
	page->SetId (std::stoul (data));
}

void BitmapFontLoader::ProcessPageTexture (const std::string& data, BitmapFontPage* page,
	const std::string& filename)
{
	std::string textureName = data;
	Extensions::StringExtend::Trim (textureName, '"');

	std::string directory = FileSystem::GetDirectory (filename);
	textureName = directory + textureName;

	/*
	 * Check if texture was not loaded before
	*/

	Texture* texture = TextureManager::Instance()->GetTexture (textureName);
	if (texture == nullptr) {
		texture = Resources::LoadTexture (textureName);
		TextureManager::Instance ()->AddTexture (texture);
	}

	page->SetTexture (texture);
}

void BitmapFontLoader::ProcessCharId (const std::string& data, BitmapFontChar* ch)
{
	ch->SetId ((unsigned char) std::stoul (data));
}

void BitmapFontLoader::ProcessCharPositionX (const std::string& data, BitmapFontChar* ch)
{
	glm::uvec2 curPos = ch->GetPosition ();
	curPos.x = std::stoul (data);

	ch->SetPosition (curPos);
}

void BitmapFontLoader::ProcessCharPositionY (const std::string& data, BitmapFontChar* ch)
{
	glm::uvec2 curPos = ch->GetPosition ();
	curPos.y = std::stoul (data);

	ch->SetPosition (curPos);
}

void BitmapFontLoader::ProcessCharSizeWidth (const std::string& data, BitmapFontChar* ch)
{
	glm::uvec2 curSize = ch->GetSize ();
	curSize.x = std::stoul (data);

	ch->SetSize (curSize);
}

void BitmapFontLoader::ProcessCharSizeHeight (const std::string& data, BitmapFontChar* ch)
{
	glm::uvec2 curSize = ch->GetSize ();
	curSize.y = std::stoul (data);

	ch->SetSize (curSize);
}

void BitmapFontLoader::ProcessCharOffsetX (const std::string& data, BitmapFontChar* ch)
{
	glm::ivec2 curOffset = ch->GetOffset ();
	curOffset.x = std::stoi (data);

	ch->SetOffset (curOffset);
}

void BitmapFontLoader::ProcessCharOffsetY (const std::string& data, BitmapFontChar* ch)
{
	glm::ivec2 curOffset = ch->GetOffset ();
	curOffset.y = std::stoi (data);

	ch->SetOffset (curOffset);
}

void BitmapFontLoader::ProcessCharXAdvance (const std::string& data, BitmapFontChar* ch)
{
	ch->SetXAdvance (std::stoi (data));
}

void BitmapFontLoader::ProcessCharPage (const std::string& data, BitmapFontChar* ch)
{
	ch->SetPage (std::stoul (data));
}

void BitmapFontLoader::ProcessCharChannel (const std::string& data, BitmapFontChar* ch)
{
	ch->SetChannel ((char) std::stoul (data));
}

std::pair<std::string, std::string> BitmapFontLoader::ProcessAttribute (
	const std::string& data, std::size_t& pos) 
{
	std::string attribute;
	std::string value;

	for (; data.size () > pos && data [pos] != '='; ++pos) {
		attribute += data [pos];
	}

	for (;std::isspace (data [pos]) || data [pos] == '='; ++ pos) {

	}

	for (; data.size () > pos && !std::isspace (data [pos]); ++pos) {
		value += data [pos];
	}

	return std::make_pair (attribute, value);
}
