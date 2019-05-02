#include "TextureLUTLoader.h"

#include "Texture/TextureLUT.h"

#include "Resources/Resources.h"

#include "Wrappers/OpenGL/GL.h"

Object* TextureLUTLoader::Load (const std::string& filename)
{
	/*
	 * Create lookup table texture
	*/

	TextureLUT* lutTexture = new TextureLUT (filename);

	/*
	 * Load texture
	*/

	Texture* texture = Resources::LoadTexture (filename);

	/*
	 * Copy data to lookup table texture
	*/

	lutTexture->SetDepth (texture->GetSize ().height);
	lutTexture->SetPixels (texture->GetPixels (), 4 * texture->GetSize ().width * texture->GetSize ().height);

	/*
	 * Delete texture
	*/

	delete texture;

	/*
	 * Load lookup table texture in GPU
	*/

	unsigned int textureID;
	GL::GenTextures (1, &textureID);
	GL::BindTexture (GL_TEXTURE_2D, textureID);

	std::size_t depth = lutTexture->GetDepth ();
	// std::size_t pixelsLength = 4 * depth * 3;

	GL::TexImage3D (GL_TEXTURE_2D, 0, GL_RGBA, depth, depth, depth, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, lutTexture->GetPixels ());

	GL::TexParameteri (GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	GL::TexParameteri (GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	GL::TexParameteri (GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	GL::TexParameteri (GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

	lutTexture->SetGPUIndex (textureID);

	return lutTexture;
}

void TextureLUTLoader::ReorganizePixels (unsigned char* pixels, std::size_t size)
{
	for (std::size_t i = 0; i < size; i++) {
		for (std::size_t j = 0; j < size; j++) {

		}
	}
}
