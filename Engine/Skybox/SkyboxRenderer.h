#ifndef SKYBOXRENDERER_H
#define SKYBOXRENDERER_H

#include "SceneNodes/Model3DRenderer.h"

#include <string>

#include "Texture/CubeMap.h"
#include "Utils/Color/Color.h"

class SkyboxRenderer : public Model3DRenderer
{
private:
	CubeMap*& _cubeMap;
	Color* _tintColor;
	float* _brightness;
	std::string _shaderName;

public:
	SkyboxRenderer (CubeMap*& map, Color* tint, float* bt, std::string shn);

	virtual void Draw ();
protected:
	void ManageCustomAttributes ();
};

#endif