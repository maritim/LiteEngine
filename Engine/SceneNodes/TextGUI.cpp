#include "TextGUI.h"

#include "Managers/TextManager.h"

TextGUI::TextGUI () :
	_text ("\0"),
	_font (nullptr),
	_screenPos (Vector3::Zero),
	_lineLength (1.0f),
	_isDirty (true),
	_textRenderer (nullptr)
{
	_textRenderer = new TextGUIRenderer (_transform);

	UpdateText ();
}

TextGUI::TextGUI (const std::string& text, Font* font, 
	Vector3 screenPos, float lineLength) :
	_text (text),
	_font (font),
	_screenPos (screenPos),
	_lineLength (lineLength),
	_isDirty (true),
	_textRenderer (nullptr)
{
	_textRenderer = new TextGUIRenderer (_transform);

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

TextGUIRenderer* TextGUI::GetTextRenderer ()
{
	return _textRenderer;
}

void TextGUI::OnAttachedToScene ()
{
	TextManager::Instance ()->AddTextElement (this);
}

void TextGUI::OnDetachedFromScene ()
{
	TextManager::Instance ()->RemoveTextElement (this);
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
	_textRenderer->UpdateText (_text, _font, _screenPos, _lineLength);
}