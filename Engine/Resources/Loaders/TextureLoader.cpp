#include "TextureLoader.h"

#include <SDL2/SDL_image.h>
#include <string>

#include "Texture/Texture.h"

#include "Core/Console/Console.h"

/*
 * TODO: Change this to another part of the code.
*/

#define TEXTURE_LOADING_ERROR_CODE 14
#define TEXTURE_CONVERTING_PIXEL_FORMAT_ERROR_CODE 16

Object* TextureLoader::Load(const std::string& filename)
{
	/*
	 * Load file using SDL2_Image::IMG_Load ()
	*/

	SDL_Surface* surface = IMG_Load (filename.c_str ());

	if (surface == nullptr) {
		Console::LogError ("Unable to load \"" + filename + "\" texture!");
		exit (TEXTURE_LOADING_ERROR_CODE);
	}

	/*
	 * Convert the surface to R8G8B8A8 pixel format
	*/

	SDL_PixelFormat pf;
	pf.palette = 0; pf.BitsPerPixel = 32; pf.BytesPerPixel = 4;
	pf.Rloss = pf.Gloss = pf.Bloss = pf.Aloss = 0;
	pf.Rmask = 0x000000ff; pf.Rshift = 0;
	pf.Gmask = 0x0000ff00; pf.Gshift = 8;
	pf.Bmask = 0x00ff0000; pf.Bshift = 16;
	pf.Amask = 0xff000000; pf.Ashift = 24;

	SDL_Surface* surface2 = SDL_ConvertSurface(surface, &pf, SDL_SWSURFACE);

	if (surface2 == nullptr) {
		Console::LogError ("Unable to format \"" + filename + "\" texture!");
		exit (TEXTURE_CONVERTING_PIXEL_FORMAT_ERROR_CODE);
	}

	SDL_FreeSurface (surface);

	if (SDL_MUSTLOCK (surface2) == SDL_TRUE) {
		SDL_LockSurface (surface2);
	}

	Console::Log (filename + " image was successully loaded!");

	Texture* texture = new Texture(filename);
	texture->SetSize (Size (surface2->w, surface2->h));
	texture->SetPixels ((const unsigned char*) surface2->pixels, 4u * surface2->w * surface2->h);

	SDL_FreeSurface (surface2);

	return texture;
}