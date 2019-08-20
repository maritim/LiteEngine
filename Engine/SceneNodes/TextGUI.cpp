#include "TextGUI.h"

#include "Renderer/RenderSystem.h"
#include "Renderer/RenderManager.h"

TextGUI::TextGUI () :
	_renderObject (new RenderTextGUIObject ())
{
	_renderObject->SetTransform (_transform);
	_renderObject->SetRenderStage (RenderStage::RENDER_STAGE_FORWARD);
}

TextGUI::~TextGUI ()
{
	delete _renderObject;
}

void TextGUI::SetFont (const Resource<Font>& font)
{
	_renderObject->SetFont (font);

	Resource<TextureView> textureView = RenderSystem::LoadTexture (font->GetTexture (0));
	_renderObject->SetFontTextureView (textureView);
}

void TextGUI::SetText (const std::string& text)
{
	_renderObject->SetText (text);
}

void TextGUI::Update ()
{

}

void TextGUI::OnAttachedToScene ()
{
	RenderManager::Instance ()->AttachRenderObject (_renderObject);
}

void TextGUI::OnDetachedFromScene ()
{
	RenderManager::Instance ()->DetachRenderObject (_renderObject);
}
