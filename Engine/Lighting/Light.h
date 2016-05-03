#ifndef LIGHT_H
#define LIGHT_H

#include "SceneGraph/SceneObject.h"

#include "Utils/Color/Color.h"

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

public:
	Light ();
	virtual ~Light ();

	Color GetColor () const;
	Color GetSpecularColor () const;

	void SetColor (const Color& color);
	void SetSpecularColor (const Color& specularColor);

	virtual void Update () = 0;
};

#endif