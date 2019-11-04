#ifndef RENDERLIGHTOBJECT_H
#define RENDERLIGHTOBJECT_H

#include "Renderer/RenderObject.h"

#include "Renderer/PipelineAttribute.h"

#include "Utils/Color/Color.h"

class RenderLightObject : public RenderObject
{
public:
	struct Shadow
	{
		glm::ivec2 resolution;
		std::size_t cascadesCount;
		float bias;

		Shadow ();
	};

protected:
	Color _lightColor;
	float _lightIntensity;
	bool _castShadows;
	Shadow _lightShadow;

public:
	RenderLightObject ();

	void Draw ();

	void SetLightColor (const Color& color);
	void SetLightIntensity (float intensity);
	void SetShadowCasting (bool castShadows);
	void SetLightShadow (const RenderLightObject::Shadow& lightShadow);

	bool IsCastingShadows () const;
	RenderLightObject::Shadow GetShadow () const;
protected:
	virtual std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif