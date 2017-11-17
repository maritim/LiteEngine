#ifndef REFLECTIVEDIRECTIONALSHADOWMAPACCUMULATIONPASS_H
#define REFLECTIVEDIRECTIONALSHADOWMAPACCUMULATIONPASS_H

#include "Renderer/RenderPassI.h"

#include "ReflectiveShadowMapVolume.h"

class ReflectiveDirectionalShadowMapAccumulationPass : public RenderPassI
{
protected:
	ReflectiveShadowMapVolume* _reflectiveShadowMapVolume;

public:
	ReflectiveDirectionalShadowMapAccumulationPass ();
	virtual ~ReflectiveDirectionalShadowMapAccumulationPass ();

	void Init ();
	RenderVolumeCollection* Execute (Scene* scene, Camera* camera, RenderVolumeCollection* rvc);
protected:
	void StartShadowMapPass ();
	void ShadowMapGeometryPass (Scene* scene, Camera* lightCamera);
	void EndShadowMapPass ();

	Camera* GetLightCamera (Scene* scene, Camera* camera);
};

#endif