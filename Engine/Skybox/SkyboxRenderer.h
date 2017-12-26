#ifndef SKYBOXRENDERER_H
#define SKYBOXRENDERER_H

#include "SceneNodes/Model3DRenderer.h"

#include <string>

#include "Skybox/Skybox.h"

class SkyboxRenderer : public Model3DRenderer
{
private:
	std::string _shaderName;
	Skybox* _skybox;

public:
	SkyboxRenderer (Skybox*);

	virtual void Draw ();
protected:
	void ManageCustomAttributes ();
};

#endif