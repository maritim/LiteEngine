#include "RenderTextGUIObject.h"

#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

#include "Managers/ShaderManager.h"

//TODO: Fix this
#include "Wrappers/OpenGL/GL.h"

RenderTextGUIObject::RenderTextGUIObject () :
	_shaderName ("FONT"),
	_font (nullptr),
	_fontTextureView (nullptr)
{
	ShaderManager::Instance ()->AddShader (_shaderName,
		"Assets/Shaders/fontVertex.glsl",
		"Assets/Shaders/fontFragment.glsl");
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

	Pipeline::SetShader (ShaderManager::Instance ()->GetShader (_shaderName));

	Pipeline::SetObjectTransform (_transform);
	Pipeline::UpdateMatrices (ShaderManager::Instance ()->GetShader (_shaderName));

	Pipeline::SendCustomAttributes (_shaderName, GetUniformAttributes ());

	_modelView->Draw ();
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
