#ifndef TEXTGUI_H
#define TEXTGUI_H

#include "SceneGraph/SceneObject.h"

#include <string>

#include "Core/Math/Vector3.h"

#include "Fonts/Font.h"

#include "TextGUIRenderer.h"

class TextGUI : public SceneObject
{
private:
	std::string _text;
	Font* _font;
	Vector3 _screenPos;
	float _lineLength;
	bool _isDirty;

public:
	TextGUI ();
	TextGUI (const std::string& text, Font* font = nullptr, 
		Vector3 screenPos = Vector3::Zero, float lineLength = 1.0f);
	~TextGUI ();

	void SetFont (Font* font);
	void SetText (const std::string& text);
	void SetScreenPosition (Vector3 screenPos);
	void SetLineLength (float lineLength);

	void Update ();
private:
	void UpdateText ();
};

#endif