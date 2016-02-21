#include "TextureLoader.h"

#include <SDL/SDL_image.h>
#include <GL/glew.h>

#include <string>

#include "Texture/Texture.h"

#include "Core/Console/Console.h"

Object* TextureLoader::Load(const std::string& filename)
{
	unsigned int num;

	glGenTextures(1,&num);
	SDL_Surface* img=IMG_Load(filename.c_str());
	
	if(img==NULL) {
		Console::LogError (filename + " image was not loaded!");
		exit (1);
	} else {
		Console::Log (filename + " image was successully loaded!");
	}

	SDL_PixelFormat pf;//={NULL,32,4,0,0,0,0,0,0,0,0,0xff000000,0x00ff0000,0x0000ff00,0x000000ff,0,255};

	pf.palette = 0;
	pf.BitsPerPixel = 32;
	pf.BytesPerPixel = 4;
	pf.alpha = 255;
	pf.Rshift = pf.Rloss = pf.Gloss = pf.Bloss = pf.Aloss = pf.colorkey = 0;
	pf.Rmask = 0x000000ff;
	pf.Gshift = 8;
	pf.Gmask = 0x0000ff00;
	pf.Bshift = 16;
	pf.Bmask = 0x00ff0000;
	pf.Ashift = 24;
	pf.Amask = 0xff000000;

	SDL_Surface* img2=SDL_ConvertSurface(img,&pf,SDL_SWSURFACE);
	if(img2==NULL) {
		Console::LogError (filename + " could not be loaded!");
		exit (1);             
	}

	glBindTexture(GL_TEXTURE_2D,num);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);        
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);      //we repeat the pixels in the edge of the texture, it will hide that 1px wide line at the edge of the cube, which you have seen in the video
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);      //we do it for vertically and horizontally (previous line)
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	glTexImage2D(GL_TEXTURE_2D,	0,GL_RGBA,img2->w,img2->h,0,GL_RGBA,GL_UNSIGNED_BYTE,img2->pixels);          
//	glTexImage2D(GL_TEXTURE_2D,	0,GL_RGBA,img2->w,img2->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img2->pixels);   

	glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_MODULATE);	//maybe just this

	SDL_FreeSurface(img);
	SDL_FreeSurface(img2);

	Texture* texture = new Texture(filename, num);

	return texture;
}