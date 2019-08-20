#ifndef RENDERLIGHTOBJECT_H
#define RENDERLIGHTOBJECT_H

#include "Renderer/RenderObject.h"

#include "Renderer/PipelineAttribute.h"

#include "Utils/Color/Color.h"

class RenderLightObject : public RenderObject
{
protected:
	Color _lightColor;
	float _lightIntensity;
	bool _castShadows;

public:
	RenderLightObject ();

	void Draw ();

	void SetLightColor (const Color& color);
	void SetLightIntensity (float intensity);
	void SetShadowCasting (bool castShadows);

	bool IsCastingShadows () const;
protected:
	virtual std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif