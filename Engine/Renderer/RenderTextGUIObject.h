#ifndef RENDERTEXTGUIOBJECT_H
#define RENDERTEXTGUIOBJECT_H

#include "Renderer/RenderObject.h"

#include "Renderer/PipelineAttribute.h"

#include "Fonts/Font.h"

class RenderTextGUIObject : public RenderObject
{
protected:
	std::string _shaderName;
	Resource<Font> _font;
	Resource<TextureView> _fontTextureView;

public:
	RenderTextGUIObject ();

	void SetFont (const Resource<Font>&);
	void SetFontTextureView (const Resource<TextureView>&);

	void SetText (const std::string& text);

	void Draw ();
protected:
	std::vector<PipelineAttribute> GetUniformAttributes ();
};

#endif