#include "TextureLoader.h"

#include <SDL2/SDL_image.h>
#include <string>

#include "Texture/Texture.h"

#include "Core/Console/Console.h"

Object* TextureLoader::Load(const std::string& filename)
{
	/*
	 * Load file using SDL2_Image::IMG_Load ()
	*/

	SDL_Surface* surface = IMG_Load (filename.c_str ());

	if (surface == nullptr) {
		Console::LogError ("Unable to load \"" + filename + "\" texture!");
		exit (1);
	}

	/*
	 * Convert the surface to R8G8B8A8 pixel format
	*/

	SDL_PixelFormat pf;
	pf.palette = 0; pf.BitsPerPixel = 32; pf.BytesPerPixel = 4;
	pf.Rshift = pf.Rloss = pf.Gloss = pf.Bloss = pf.Aloss = 0;
	pf.Rmask = 0x000000ff; pf.Gshift = 8;
	pf.Gmask = 0x0000ff00; pf.Bshift = 16;
	pf.Bmask = 0x00ff0000; pf.Ashift = 24;
	pf.Amask = 0xff000000;

	SDL_Surface* surface2 = SDL_ConvertSurface(surface, &pf, SDL_SWSURFACE);

	if (surface2 == nullptr) {
		Console::LogError ("Unable to format \"" + filename + "\" texture!");
		exit (1);
	}

	SDL_FreeSurface (surface);

	Console::Log (filename + " image was successully loaded!");

	Texture* texture = new Texture(filename, surface2);

	return texture;
}