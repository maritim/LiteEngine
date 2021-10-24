#include "PNGSaver.h"

#include <cstring>
#include <SDL2/SDL_image.h>

#include "Renderer/Render/Texture/Texture.h"

#include "Core/Console/Console.h"

bool PNGSaver::Save (const Object* object, const std::string& filename)
{
	const Texture* texture = dynamic_cast<const Texture*> (object);

	if (texture == nullptr) {
		Console::LogError ("Could not save \"" + filename + "\" texture!");
		return false;
	}

	auto size = texture->GetSize ();

	SDL_Surface* surface = SDL_CreateRGBSurface(0, size.width, size.height, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	if (surface == nullptr) {
		Console::LogError ("Could not save \"" + filename + "\" texture!");
		return false;
	}

	unsigned char* pixels = new unsigned char [size.width * size.height * 4];
	std::memcpy (pixels, texture->GetPixels (), size.width * size.height * 4);

	std::int32_t* pixelsI = (std::int32_t*) pixels;
	for (std::size_t i = 0; i < size.height / 2; i ++) {
		for (std::size_t j = 0; j < size.width; j ++) {
			std::swap (pixelsI [i * size.width + j], pixelsI [(size.height - i - 1) * size.width + j]);
		}
	}

	surface->pixels = pixels;

	int result = IMG_SavePNG (surface, filename.c_str ());

	if (result == 0) {
		Console::Log ("Image \"" + filename + "\" was successully saved!");
	} else {
		Console::LogError ("Could not save \"" + filename + "\" texture!");
	}

	delete[] pixels;

	surface->pixels = nullptr;
	SDL_FreeSurface (surface);

	return true;
}
