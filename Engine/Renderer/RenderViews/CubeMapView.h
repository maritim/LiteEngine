#ifndef CUBEMAPVIEW_H
#define CUBEMAPVIEW_H

#include "TextureView.h"

class CubeMapView : public TextureView
{
public:
	void Activate (std::size_t textureUnit);
};

#endif