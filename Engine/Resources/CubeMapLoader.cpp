#include <SDL/SDL_image.h>
#include <GL/glew.h>

#include "CubeMapLoader.h"

#include <string>
#include <vector>

#include "Texture/CubeMap.h"

#include "Core/Console/Console.h"

Object* CubeMapLoader::Load(const std::string& filename)
{
	unsigned int tex;
	glGenTextures (1, &tex);
	glBindTexture (GL_TEXTURE_CUBE_MAP, tex);

	CubeMap* cubeMap = new CubeMap(filename, 0);

	for (std::size_t i=0;i<6;i++) {
		unsigned int num;
		glGenTextures(1,&num);

	    cubeMap->SetFilename(_filenames[i], i);
	    cubeMap->SetGPUIndexFace(num, i);

		SDL_Surface* img=IMG_Load(_filenames [i].c_str ());
		
		if(img==NULL) {
			Console::LogError (_filenames [i] + " img was not loaded!");
			exit (1);
		}

		SDL_PixelFormat form={NULL,32,4,0,0,0,0,0,0,0,0,0xff000000,0x00ff0000,0x0000ff00,0x000000ff,0,255};
		SDL_Surface* img2=SDL_ConvertSurface(img,&form,SDL_SWSURFACE);

		if (img == NULL) {
			Console::LogError (_filenames [i] + " could not be loaded!");
			exit (1);
		}

		SDL_FreeSurface (img);

		glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, img2->w, img2->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, img2->pixels);

		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);        
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);      //we repeat the pixels in the edge of the texture, it will hide that 1px wide line at the edge of the cube, which you have seen in the video
	    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);      //we do it for vertically and horizontally (previous line)		

	    SDL_FreeSurface (img2);
	}

	glBindTexture (GL_TEXTURE_CUBE_MAP, 0);

	cubeMap->SetGPUIndex (tex);

	return cubeMap;
}

void CubeMapLoader::SetFilenames(std::vector<std::string> filenames)
{
	_filenames = filenames;
}