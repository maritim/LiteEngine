#ifndef LIGHT_H
#define LIGHT_H

#include "SceneGraph/SceneObject.h"

#include "Utils/Color/Color.h"

/*
 * Color: light color
*/

class Light : public SceneObject
{
protected:
	Color _color;
	bool _castShadows;

public:
	Light ();
	virtual ~Light ();

	Color GetColor () const;
	bool IsCastingShadows () const;

	void SetColor (const Color& color);
	void SetShadowCasting (bool castShadows);

	virtual void Update () = 0;
};

#endif