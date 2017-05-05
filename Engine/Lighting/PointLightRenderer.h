#ifndef POINTLIGHTRENDERER_H
#define POINTLIGHTRENDERER_H

#include "LightRenderer.h"

class PointLightRenderer : public LightRenderer
{
public:
	PointLightRenderer (Light* light);
protected:
	std::vector<PipelineAttribute> GetCustomAttributes ();
};

#endif