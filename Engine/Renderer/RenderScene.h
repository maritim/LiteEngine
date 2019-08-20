#ifndef RENDERSCENE_H
#define RENDERSCENE_H

#include <set>

#include "RenderObject.h"
#include "RenderSkyboxObject.h"
#include "RenderDirectionalLightObject.h"
#include "RenderPointLightObject.h"
#include "RenderAmbientLightObject.h"

#include "Core/Intersections/AABBVolume.h"

class RenderScene
{
protected:
	std::set<RenderObject*> _renderObjects;
	RenderSkyboxObject* _renderSkyboxObject;

	RenderDirectionalLightObject* _renderDirectionalLightObject;
	std::set<RenderPointLightObject*> _renderPointLightObjects;
	RenderAmbientLightObject* _renderAmbientLightObject;

	AABBVolume* _boundingBox;

public:
	RenderScene ();
	~RenderScene ();

	void SetRenderSkyboxObject (RenderSkyboxObject*);

	void AttachRenderObject (RenderObject*);
	void DetachRenderObject (RenderObject*);

	void SetRenderDirectionalLightObject (RenderDirectionalLightObject*);

	void AttachRenderPointLightObject (RenderPointLightObject*);
	void DetachRenderPointLightObject (RenderPointLightObject*);

	void SetRenderAmbientLightObject (RenderAmbientLightObject*);

	RenderSkyboxObject* GetRenderSkyboxObject () const;
	RenderDirectionalLightObject* GetRenderDirectionalLightObject () const;
	RenderAmbientLightObject* GetRenderAmbientLightObject () const;
	AABBVolume* GetBoundingBox () const;

	std::set<RenderObject*>::const_iterator begin () const;
	std::set<RenderObject*>::const_iterator end () const;
protected:
	void UpdateBoundingBox ();
};

#endif