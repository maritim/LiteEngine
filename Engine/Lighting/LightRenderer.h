#ifndef LIGHTRENDERER_H
#define LIGHTRENDERER_H

#include "SceneNodes/Model3DRenderer.h"

#include <vector>

#include "SceneGraph/Transform.h"

#include "Renderer/PipelineAttribute.h"
#include "Lighting/Light.h"

class LightRenderer : public Model3DRenderer
{
protected:
	Light* _light;
	std::string _shaderName;

public:
	LightRenderer (Light* light);

	void Draw ();

	virtual std::vector<PipelineAttribute> GetCustomAttributes ();
};

#endif