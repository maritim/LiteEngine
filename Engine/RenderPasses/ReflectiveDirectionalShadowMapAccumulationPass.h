#ifndef REFLECTIVEDIRECTIONALSHADOWMAPACCUMULATIONPASS_H
#define REFLECTIVEDIRECTIONALSHADOWMAPACCUMULATIONPASS_H

#include "VolumetricLightContainerRenderSubPassI.h"

#include <string>

#include "ReflectiveShadowMapVolume.h"

class ReflectiveDirectionalShadowMapAccumulationPass : public VolumetricLightContainerRenderSubPassI
{
protected:
	std::string _staticShaderName;
	std::string _animationShaderName;
	ReflectiveShadowMapVolume* _reflectiveShadowMapVolume;

public:
	ReflectiveDirectionalShadowMapAccumulationPass ();
	virtual ~ReflectiveDirectionalShadowMapAccumulationPass ();

	void Init ();
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

	bool IsAvailable (const VolumetricLight*) const;
protected:
	void StartShadowMapPass ();
	void ShadowMapGeometryPass (const Scene* scene, const Camera* lightCamera);
	void EndShadowMapPass ();

	void LockShader (int sceneLayers);

	Camera* GetLightCamera (const Scene* scene, const Camera* camera);
};

#endif