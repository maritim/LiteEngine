#include "TextManager.h"

#include <algorithm>

#include "ShaderManager.h"

#include "Core/Console/Console.h"

TextManager::TextManager () :
	_charLimits (2000)
{
	ShaderManager::Instance ()->AddShader ("DEFAULT_FONT", "Assets/Shaders/fontVertex.glsl", "Assets/Shaders/fontFragment.glsl");

	_shaderName = "DEFAULT_FONT";
}

TextManager::~TextManager ()
{
	
}

void TextManager::AddTextElement (TextGUI* textElement)
{
	_textElements.push_back (textElement);
}

void TextManager::RemoveTextElement (TextGUI* textElement)
{
	auto it = std::find (_textElements.begin (), _textElements.end (), textElement);

	if (it == _textElements.end ()) {
		return ;
	}

	_textElements.erase (it);
}

TextGUI* TextManager::GetTextElement (std::size_t index)
{
	if (index < 0 || _textElements.size () <= index) {
		Console::LogWarning ("TextManager::_textElements bounds exceeded. Aborting...");
		return nullptr;
	}

	return _textElements [index];
}

std::size_t TextManager::GetTextElementsCount ()
{
	return _textElements.size ();
}

std::size_t TextManager::GetCharLimits ()
{
	return _charLimits;
}

std::string TextManager::GetShaderName ()
{
	return _shaderName;
}
