#ifndef SKYBOX_H
#define SKYBOX_H

#include "SceneGraph/SceneObject.h"

#include "Renderer/RenderSkyboxObject.h"

#include "Texture/Texture.h"

// TODO: Extend this
// TODO: Refactor shader management for this (maybe GameSettings.xml?)
class Skybox : public SceneObject
{
private:
	RenderSkyboxObject* _renderSkyboxObject;

public:
	Skybox ();
	~Skybox ();

	void SetCubeMap (const Resource<Texture>& cubeMap);

	void SetTintColor (Color color);

	void SetBrightness (float brightness);

	void SetAngularVelocity (float velocity);

	void OnAttachedToScene ();
	void OnDetachedFromScene ();
};

#endif