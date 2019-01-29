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

#include "SceneGraph/Scene.h"
#include "Systems/Camera/Camera.h"

#include "RenderModule.h"

#include <vector>

enum RenderMode {
	RENDER_MODE_DIRECT_LIGHTING = 0,
	RENDER_MODE_LIGHT_MAP,
	RENDER_MODE_VOXELIZATION,
	RENDER_MODE_VOXEL_CONE_TRACE,
	RENDER_MODE_REFLECTIVE_SHADOW_MAP
};

class RenderManager : public Singleton<RenderManager>
{
	friend class Singleton<RenderManager>;

private:
	RenderModule* _currentRenderModule;
	RenderMode _currentRenderMode;
	bool _needToChangeRenderMode;
	std::vector<RenderModule*> _renderModules;

public:
	void Init ();

	void SetRenderMode (RenderMode);
	RenderMode GetRenderMode () const;

	void RenderScene (const Scene* scene, const Camera* camera);

	void Clear ();
private:
	RenderManager ();
	RenderManager (const RenderManager& other);
	RenderManager& operator=(const RenderManager& other);
	~RenderManager ();

	void SwitchRenderModule ();
};

#endif