#ifndef LIGHT_H
#define LIGHT_H

#include "SceneGraph/SceneObject.h"

#include "Utils/Color/Color.h"

/*
 * Color: light color
*/

class ENGINE_API Light : public SceneObject
{
protected:
	Color _color;
	float _intensity;
	bool _castShadows;

public:
	Light ();
	virtual ~Light ();

	Color GetColor () const;
	float GetIntensity () const;
	bool IsCastingShadows () const;

	virtual void SetColor (const Color& color);
	virtual void SetIntensity (float intensity);
	virtual void SetShadowCasting (bool castShadows);

	virtual void Update () = 0;
};

#endif