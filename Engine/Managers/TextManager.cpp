#include "TextManager.h"

#include "ShaderManager.h"

TextManager::TextManager () :
	_charLimits (2000)
{
	ShaderManager::Instance ()->AddShader ("DEFAULT_FONT", "Assets/Shaders/fontVertex.glsl", "Assets/Shaders/fontFragment.glsl");

	_shaderName = "DEFAULT_FONT";
}

TextManager::~TextManager ()
{
	
}

std::size_t TextManager::GetCharLimits ()
{
	return _charLimits;
}

std::string TextManager::GetShaderName ()
{
	return _shaderName;
}
