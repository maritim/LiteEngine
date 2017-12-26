#ifndef SKYBOX_H
#define SKYBOX_H

#include "Texture/CubeMap.h"

#include "SceneNodes/GameObject.h"
#include "Utils/Color/Color.h"

// TODO: Extend this
// TODO: Refactor shader management for this (maybe GameSettings.xml?)
class Skybox : public GameObject
{
public:
	enum Comp {SKY_LEFT = 0, SKY_RIGHT, SKY_UP, SKY_DOWN, SKY_FRONT, SKY_BACK};

private:
	CubeMap* _cubemap;
	Color _tintColor;
	float _brightness;
	float _angularVelocity;

public:
	Skybox ();
	~Skybox ();

	void SetCubeMap (CubeMap* cubeMap);
	CubeMap* GetCubeMap () const;

	void SetTintColor (Color color);
	Color GetTintColor () const;

	void SetBrightness (float brightness);
	float GetBrightness () const;

	void SetAngularVelocity (float velocity);
	float GetAngularVelocity () const;
};

#endif