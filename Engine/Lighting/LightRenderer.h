#ifndef LIGHTRENDERER_H
#define LIGHTRENDERER_H

#include "SceneNodes/Model3DRenderer.h"

#include <vector>

#include "SceneGraph/Scene.h"
#include "SceneGraph/Transform.h"

#include "Systems/Camera/Camera.h"

#include "Renderer/PipelineAttribute.h"
#include "Renderer/GBuffer.h"
#include "Lighting/Light.h"

class LightRenderer : public Model3DRenderer
{
protected:
	Light* _light;
	std::string _shaderName;

public:
	LightRenderer (Light* light);
	virtual ~LightRenderer ();

	virtual void Draw (Scene*, Camera*, GBuffer* gBuffer = nullptr);
protected:
	virtual std::vector<PipelineAttribute> GetCustomAttributes ();
};

#endif