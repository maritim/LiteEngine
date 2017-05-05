#ifndef DIRECTIONALLIGHTRENDERER_H
#define DIRECTIONALLIGHTRENDERER_H

#include "LightRenderer.h"

class DirectionalLightRenderer : public LightRenderer
{
public:
	DirectionalLightRenderer (Light* light);
	~DirectionalLightRenderer ();
};

#endif