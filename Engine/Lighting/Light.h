#ifndef LIGHT_H
#define LIGHT_H

#include "SceneGraph/SceneObject.h"

#include "Utils/Color/Color.h"

/*
 * Color: light color
*/

class ENGINE_API Light : public SceneObject
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
	Color _color;
	float _intensity;
	bool _castShadows;
	Shadow _shadow;

public:
	Light ();
	virtual ~Light ();

	Color GetColor () const;
	float GetIntensity () const;
	bool IsCastingShadows () const;
	Light::Shadow GetShadow () const;

	virtual void SetColor (const Color& color);
	virtual void SetIntensity (float intensity);
	virtual void SetShadowCasting (bool castShadows);
	virtual void SetShadow (const Light::Shadow& shadow);

	virtual void Update () = 0;
};

#endif