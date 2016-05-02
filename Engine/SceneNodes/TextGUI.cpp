#include "TextGUI.h"

#include "TextGUIRenderer.h"

TextGUI::TextGUI () :
	_text ("\0"),
	_font (nullptr),
	_screenPos (Vector3::Zero),
	_lineLength (1.0f),
	_isDirty (true)
{

}

TextGUI::TextGUI (const std::string& text, Font* font, 
	Vector3 screenPos, float lineLength) :
	_text (text),
	_font (font),
	_screenPos (screenPos),
	_lineLength (lineLength),
	_isDirty (true)
{
	delete _renderer;
	_renderer = new TextGUIRenderer (_transform);
	_renderer->SetPriority (5);

	UpdateText ();
}

TextGUI::~TextGUI ()
{

}

void TextGUI::SetFont (Font* font)
{
	_font = font;
	_isDirty = true;
}

void TextGUI::SetText (const std::string& text)
{
	_text = text;
	_isDirty = true;
}

void TextGUI::SetScreenPosition (Vector3 position)
{
	_screenPos = position;
	_isDirty = true;
}

void TextGUI::SetLineLength (float lineLength)
{
	_lineLength = lineLength;
	_isDirty = true;
}

void TextGUI::Update ()
{
	if (!_isDirty) {
		return ;
	}

	UpdateText ();
	_isDirty = false;
}

void TextGUI::UpdateText ()
{
	TextGUIRenderer* renderer = dynamic_cast<TextGUIRenderer*> (_renderer);
	renderer->UpdateText (_text, _font, _screenPos, _lineLength);
}