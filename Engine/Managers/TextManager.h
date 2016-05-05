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

	std::vector<TextGUI*> _textElements;

public:
	std::size_t GetCharLimits ();
	std::string GetShaderName ();

	void AddTextElement (TextGUI* textElement);
	void RemoveTextElement (TextGUI* textElement);

	TextGUI* GetTextElement (std::size_t pos);
	std::size_t GetTextElementsCount ();
	private:
		TextManager ();
		TextManager (const TextManager& other);
		TextManager& operator=(const TextManager& other);
		~TextManager ();
};
	
#endif