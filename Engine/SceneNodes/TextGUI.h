#ifndef TEXTGUI_H
#define TEXTGUI_H

#include "SceneGraph/SceneObject.h"

#include <glm/vec3.hpp>
#include <string>

#include "Fonts/Font.h"

#include "TextGUIRenderer.h"

class TextGUI : public SceneObject
{
private:
	std::string _text;
	Font* _font;
	glm::vec2 _screenPos;
	float _lineLength;
	bool _isDirty;

public:
	TextGUI ();
	TextGUI (const std::string& text, Font* font = nullptr, 
		glm::vec2 screenPos = glm::vec2 (0), float lineLength = 1.0f);
	~TextGUI ();

	void SetFont (Font* font);
	void SetText (const std::string& text);
	void SetScreenPosition (glm::vec2 screenPos);
	void SetLineLength (float lineLength);

	void Update ();
private:
	void UpdateText ();
};

#endif