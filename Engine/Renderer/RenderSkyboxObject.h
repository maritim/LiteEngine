#ifndef RENDERSKYBOXOBJECT_H
#define RENDERSKYBOXOBJECT_H

#include "Renderer/RenderObject.h"

#include "Renderer/RenderViews/TextureView.h"
#include "Renderer/RenderViews/ShaderView.h"
#include "Utils/Color/Color.h"

#include "Renderer/PipelineAttribute.h"

class RenderSkyboxObject : public RenderObject
{
protected:
	Resource<ShaderView> _shaderView;
	Resource<TextureView> _cubemapView;
	Color _tintColor;
	float _brightness;
	float _angularVelocity;

public:
	RenderSkyboxObject ();
	~RenderSkyboxObject ();

	void Draw ();

	void SetCubeMap (const Resource<TextureView>& cubeMap);

	void SetTintColor (Color color);

	void SetBrightness (float brightness);

	void SetAngularVelocity (float velocity);
protected:
	std::vector<PipelineAttribute> GetCustomAttributes ();
};

#endif