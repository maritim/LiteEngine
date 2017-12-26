#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>

#include "Texture/CubeMap.h"
#include "Mesh/Model.h"

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

	static Skybox* _currentSkybox;

public:
	Skybox ();
	~Skybox ();

	static void Set (Skybox* skybox);
	static void Render ();

	static std::string DefaultShaderName ();
	static CubeMap* GetCurrentCubeMap ();

	void SetCubeMap (CubeMap* cubeMap);
	CubeMap* GetCubeMap () const;

	void SetTintColor (Color color);
	Color GetTintColor () const;

	void SetBrightness (float brightness);
	float GetBrightness () const;

	void SetAngularVelocity (float velocity);
	float GetAngularVelocity () const;
private:
	static void Clear ();
};

#endif