#ifndef RENDERSKYBOXOBJECT_H
#define RENDERSKYBOXOBJECT_H

#include "Renderer/RenderObject.h"

#include "Renderer/RenderViews/TextureView.h"
#include "Utils/Color/Color.h"

#include "Renderer/PipelineAttribute.h"

class RenderSkyboxObject : public RenderObject
{
protected:
	Resource<TextureView> _cubemapView;
	Color _tintColor;
	float _brightness;
	float _angularVelocity;
	std::string _shaderName;

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