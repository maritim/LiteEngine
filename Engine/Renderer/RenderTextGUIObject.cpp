#include "RenderTextGUIObject.h"

#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

//TODO: Fix this
#include "Wrappers/OpenGL/GL.h"

RenderTextGUIObject::RenderTextGUIObject () :
	_font (nullptr),
	_fontTextureView (nullptr)
{
	/*
	 * Shader for text GUI render pass
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/fontVertex.glsl",
		"Assets/Shaders/fontFragment.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);
}

void RenderTextGUIObject::SetFont (const Resource<Font>& font)
{
	_font = font;
}

void RenderTextGUIObject::SetFontTextureView (const Resource<TextureView>& fontTextureView)
{
	_fontTextureView = fontTextureView;
}

void RenderTextGUIObject::SetText (const std::string& text)
{
	_modelView = RenderSystem::LoadTextGUI (text, _font);
}

void RenderTextGUIObject::Draw ()
{
	if (_modelView == nullptr) {
		return;
	}

	GL::Disable (GL_CULL_FACE);
	GL::Disable (GL_DEPTH_TEST);

	GL::Enable (GL_BLEND);
	GL::BlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Pipeline::LockShader (_shaderView);

	Pipeline::SetObjectTransform (_transform);
	Pipeline::UpdateMatrices (_shaderView);

	Pipeline::SendCustomAttributes (_shaderView, GetUniformAttributes ());

	_modelView->DrawGeometry ();

	Pipeline::UnlockShader ();
}

std::vector<PipelineAttribute> RenderTextGUIObject::GetUniformAttributes ()
{
	// Create attribute
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute fontTexture;

	fontTexture.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	fontTexture.name = "FontTexture";

	fontTexture.value.x = _fontTextureView->GetGPUIndex ();

	attributes.push_back (fontTexture);

	return attributes;
}
