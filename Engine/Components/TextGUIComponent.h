#ifndef TEXTGUICOMPONENT_H
#define TEXTGUICOMPONENT_H

#include "Systems/Components/Component.h"

#include "Renderer/RenderTextGUIObject.h"

class ENGINE_API TextGUIComponent : public Component
{
	DECLARE_COMPONENT(TextGUIComponent)

protected:
	ATTRIBUTE(EditAnywhere, Meta=(Font))
	Resource<Font> _font;
	ATTRIBUTE(EditAnywhere, Meta)
	std::string _text;

	RenderTextGUIObject* _renderObject;

public:
	TextGUIComponent ();
	~TextGUIComponent ();

	void Awake ();

	void SetActive (bool isActive);

	void SetFont (const Resource<Font>& font);
	void SetText (const std::string& text);

	void OnAttachedToScene ();
	void OnDetachedFromScene ();
};

#endif