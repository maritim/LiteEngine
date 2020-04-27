#ifndef RENDERSCENE_H
#define RENDERSCENE_H

#include <set>

#include "RenderObject.h"
#include "RenderSkyboxObject.h"
#include "RenderDirectionalLightObject.h"
#include "RenderPointLightObject.h"
#include "RenderSpotLightObject.h"
#include "RenderAmbientLightObject.h"

#include "Core/Intersections/AABBVolume.h"

class RenderScene
{
protected:
	std::set<RenderObject*> _renderObjects;
	RenderSkyboxObject* _renderSkyboxObject;

	std::set<RenderDirectionalLightObject*> _renderDirectionalLightObjects;
	std::set<RenderPointLightObject*> _renderPointLightObjects;
	std::set<RenderSpotLightObject*> _renderSpotLightObjects;
	RenderAmbientLightObject* _renderAmbientLightObject;

	AABBVolume _boundingBox;

public:
	RenderScene ();
	~RenderScene ();

	void SetRenderSkyboxObject (RenderSkyboxObject*);

	void AttachRenderObject (RenderObject*);
	void DetachRenderObject (RenderObject*);

	void AttachRenderDirectionalLightObject (RenderDirectionalLightObject*);
	void DetachRenderDirectionalLightObject (RenderDirectionalLightObject*);

	void AttachRenderPointLightObject (RenderPointLightObject*);
	void DetachRenderPointLightObject (RenderPointLightObject*);

	void AttachRenderSpotLightObject (RenderSpotLightObject*);
	void DetachRenderSpotLightObject (RenderSpotLightObject*);

	void SetRenderAmbientLightObject (RenderAmbientLightObject*);

	RenderSkyboxObject* GetRenderSkyboxObject () const;
	RenderAmbientLightObject* GetRenderAmbientLightObject () const;
	const AABBVolume& GetBoundingBox () const;

	MULTIPLE_CONTAINER_TEMPLATE (set)
protected:
	void UpdateBoundingBox ();
};

MULTIPLE_CONTAINER_SPECIALIZATION (set, RenderObject*, RenderScene, _renderObjects);
MULTIPLE_CONTAINER_SPECIALIZATION (set, RenderDirectionalLightObject*, RenderScene, _renderDirectionalLightObjects);
MULTIPLE_CONTAINER_SPECIALIZATION (set, RenderPointLightObject*, RenderScene, _renderPointLightObjects);
MULTIPLE_CONTAINER_SPECIALIZATION (set, RenderSpotLightObject*, RenderScene, _renderSpotLightObjects);

#endif