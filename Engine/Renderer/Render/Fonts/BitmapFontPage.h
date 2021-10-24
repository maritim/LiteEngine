#ifndef BITMAPFONTPAGE_H
#define BITMAPFONTPAGE_H

#include "Core/Interfaces/Object.h"

#include "BitmapFontCharset.h"

#include "Core/Resources/Resource.h"
#include "Renderer/Render/Texture/Texture.h"
#include "Renderer/RenderViews/TextureView.h"

class BitmapFontPage : public Object
{
private:
	std::size_t _id;
	BitmapFontCharset* _charset;
	Resource<Texture> _texture;
	Resource<TextureView> _textureView;

public:
	BitmapFontPage ();
	~BitmapFontPage ();

	std::size_t GetId () const;
	BitmapFontCharset* GetCharset () const;
	Resource<Texture> GetTexture () const;
	// Resource<Texture> GetTextureView () const;

	void SetId (std::size_t id);
	void SetCharset (BitmapFontCharset* charset);
	void SetTexture (const Resource<Texture>& texture);
};

#endif