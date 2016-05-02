#include <SDL2/SDL_image.h>
#include <GL/glew.h>

#include "CubeMapLoader.h"

#include <string>
#include <vector>

#include "Texture/CubeMap.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Console/Console.h"

Object* CubeMapLoader::Load(const std::string& filename)
{
	unsigned int tex;
	GL::GenTextures (1, &tex);
	GL::BindTexture (GL_TEXTURE_CUBE_MAP, tex);

	CubeMap* cubeMap = new CubeMap(filename, 0);

	for (std::size_t i=0;i<6;i++) {
		unsigned int num;
		GL::GenTextures(1,&num);

		cubeMap->SetFilename(_filenames[i], i);
		cubeMap->SetGPUIndexFace(num, i);

		SDL_Surface* img=IMG_Load(_filenames [i].c_str());
		
		if (img == nullptr) {
			Console::LogError (_filenames [i] + " img was not loaded!");
			exit (1);
		} else {
			Console::Log (_filenames [i] + " image was successully loaded!");
		}

		SDL_PixelFormat pf;// = {NULL,32,4,0,0,0,0,0,0,0,0,0xff000000,0x00ff0000,0x0000ff00,0x000000ff,0,255};

		pf.palette = 0;
		pf.BitsPerPixel = 32;
		pf.BytesPerPixel = 4;
		pf.Rshift = pf.Rloss = pf.Gloss = pf.Bloss = pf.Aloss = 0;
		pf.Rmask = 0xff000000;
		pf.Gshift = 8;
		pf.Gmask = 0x00ff0000;
		pf.Bshift = 16;
		pf.Bmask = 0x0000ff00;
		pf.Ashift = 24;
		pf.Amask = 0x000000ff;

		SDL_Surface* img2 = SDL_ConvertSurface(img,&pf,SDL_SWSURFACE);

		if (img2 == nullptr) {
			Console::LogError (_filenames [i] + " could not be loaded!");
			exit (1);
		}

		GL::BindTexture(GL_TEXTURE_2D,num);

		GL::TexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, img2->w, img2->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, img2->pixels);

		GL::TexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		GL::TexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);        
		GL::TexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);      //we repeat the pixels in the edge of the texture, it will hide that 1px wide line at the edge of the cube, which you have seen in the video
		GL::TexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);      //we do it for vertically and horizontally (previous line)		

		SDL_FreeSurface (img);
		SDL_FreeSurface (img2);
	}

	GL::BindTexture (GL_TEXTURE_CUBE_MAP, 0);

	cubeMap->SetGPUIndex (tex);

	return cubeMap;
}

void CubeMapLoader::SetFilenames(std::vector<std::string> filenames)
{
	_filenames = filenames;
}