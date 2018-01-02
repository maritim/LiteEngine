#ifndef LIGHT_H
#define LIGHT_H

#include "SceneGraph/SceneObject.h"

#include "Utils/Color/Color.h"

enum LightRenderMode 
{
	DEFERRED = 0,
	VOXEL_CONE_TRACE,
	REFLECTIVE_SHADOW_MAP
};

/*
 * Color: light color
 *
 * Specular Color: may differ to color
*/

class Light : public SceneObject
{
protected:
	Color _color;
	Color _specularColor;
	bool _castShadows;

public:
	Light ();
	virtual ~Light ();

	Color GetColor () const;
	Color GetSpecularColor () const;
	bool IsCastingShadows () const;

	void SetColor (const Color& color);
	void SetSpecularColor (const Color& specularColor);
	virtual void SetShadowCasting (bool castShadows);

	virtual void Update () = 0;
};

#endif