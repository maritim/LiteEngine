/* 
 * I have a render manager that will draw all my objects.  
 * That way I can sort them by shader, texture etc so that 
 * there are less state switches on the graphics card.
 *
 * Any object that is drawn has a render component that holds 
 * all the required information.  The render systems goes over 
 * all objects that have a render component and creates a 
 * RenderItem for each one that contains a Mesh, Texture and 
 * Effect pointer, along with a Tranformation Matrix.  
 * The render manager then sorts and draws all objects.
 *
 * I am only doing basic sorting at the moment but 
 * later when I have more objects I will implement 
 * culling which will save time by not drawing 
 * objects that can't be seen
*/

#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <vector>

#include "SceneGraph/Scene.h"
#include "Systems/Camera/Camera.h"

#include "Renderer.h"
#include "GBuffer.h"

class RenderManager : public Singleton<RenderManager>
{
	friend class Singleton<RenderManager>;

private:
	GBuffer* _frameBuffer;

public:
	void RenderScene (Scene* scene, Camera* camera);
private:
	RenderManager ();
	RenderManager (const RenderManager& other);
	RenderManager& operator=(const RenderManager& other);
	~RenderManager ();

	void UpdateCamera (Camera* camera);
	void UpdateGBuffer ();

	void PrepareDrawing ();
	void GeometryPass (Scene* scene);
	void LightPass ();
	void SkyboxPass ();
	void ParticleSystemPass ();
	void GUIPass ();
	void EndDrawing ();

	void DirectionalLightPass ();
	void PointLightPass ();

	void PointLightStencilPass ();
	void PointLightDrawPass ();
};

#endif