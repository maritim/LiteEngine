#include "TextGUIComponent.h"

#include "Renderer/RenderSystem.h"
#include "Renderer/RenderManager.h"

TextGUIComponent::TextGUIComponent () :
	_font (nullptr),
	_renderObject (new RenderTextGUIObject ())
{

}

TextGUIComponent::~TextGUIComponent ()
{
	delete _renderObject;
}

void TextGUIComponent::Awake ()
{
	_renderObject->SetTransform (_parent->GetTransform ());
	_renderObject->SetRenderStage (RenderStage::RENDER_STAGE_FORWARD);

	SetFont (_font);
	SetText (_text);
}

void TextGUIComponent::SetActive (bool isActive)
{
	_renderObject->SetActive (isActive);
}

void TextGUIComponent::SetFont (const Resource<Font>& font)
{
	_font = font;

	_renderObject->SetFont (font);

	Resource<TextureView> textureView = RenderSystem::LoadTexture (font->GetTexture (0));
	_renderObject->SetFontTextureView (textureView);
}

void TextGUIComponent::SetText (const std::string& text)
{
	_text = text;

	_renderObject->SetText (_text);
}

void TextGUIComponent::OnAttachedToScene ()
{
	RenderManager::Instance ()->AttachRenderObject (_renderObject);
}

void TextGUIComponent::OnDetachedFromScene ()
{
	RenderManager::Instance ()->DetachRenderObject (_renderObject);
}
