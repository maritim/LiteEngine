#ifndef TEXTGUI_H
#define TEXTGUI_H

#include "SceneGraph/SceneObject.h"

#include "Fonts/Font.h"

#include "Renderer/RenderTextGUIObject.h"

class ENGINE_API TextGUI : public SceneObject
{
private:
	RenderTextGUIObject* _renderObject;

public:
	TextGUI ();
	~TextGUI ();

	void SetFont (const Resource<Font>& font);
	void SetText (const std::string& text);
	void SetScreenPosition (glm::vec2 screenPos);
	void SetLineLength (float lineLength);

	void Update ();

	void OnAttachedToScene ();
	void OnDetachedFromScene ();
};

#endif