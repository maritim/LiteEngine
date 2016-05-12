#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <string>
#include <vector>

#include "SceneNodes/TextGUI.h"

class TextManager : public Singleton<TextManager>
{
	friend class Singleton<TextManager>;

private:
	std::size_t _charLimits;
	std::string _shaderName;

public:
	std::size_t GetCharLimits ();
	std::string GetShaderName ();
private:
	TextManager ();
	TextManager (const TextManager& other);
	TextManager& operator=(const TextManager& other);
	~TextManager ();
};
	
#endif