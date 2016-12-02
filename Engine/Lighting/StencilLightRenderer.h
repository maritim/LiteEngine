#ifndef STENCILLIGHTRENDERER_H
#define STENCILLIGHTRENDERER_H

#include "LightRenderer.h"

class StencilLightRenderer : public LightRenderer
{
protected:
	std::string _stencilShaderName;

public:
	StencilLightRenderer (Light* light);

	virtual void StencilPass ();
};

#endif