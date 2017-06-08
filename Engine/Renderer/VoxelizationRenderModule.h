#ifndef VOXELIZATIONRENDERMODULE_H
#define VOXELIZATIONRENDERMODULE_H

#include "RenderModule.h"

#include "VoxelVolume.h"

#include "Shadows/ShadowMapDirectionalLightVolume.h"

class VoxelizationRenderModule : virtual public RenderModule
{
protected:
	VoxelVolume* _voxelVolume;
	ShadowMapDirectionalLightVolume* _shadowMapVolume;
	Camera* lightCamera;

public:
	VoxelizationRenderModule ();
	virtual ~VoxelizationRenderModule ();

	virtual void RenderScene (Scene*, Camera*);
protected:
	void UpdateCamera (Camera*);

	void VoxelizePass (Scene*, Camera*);
	void VoxelRayTracePass ();

	void VoxelPass (Scene*);
	void RadianceInjectionPass (Scene* scene, Camera* camera);
	void MipmappingPass ();

	void ShadowMapPass (Scene* scene, Camera* camera);
	void RadianceShadowMapInjectionPass ();

	void StartVoxelization ();
	void GeometryVoxelizationPass (Scene* scene);
	void EndVoxelization ();

	void StartVoxelMipmaping ();
	void VoxelMipmapPass ();
	void EndVoxelMipmaping ();

	void StartShadowMapPass ();
	void ShadowMapGeometryPass (Scene* scene, Camera* lightCamera);
	void EndShadowMapPass ();

	void StartRadianceInjectionPass ();
	void RadianceInjectPass ();
	void UpdateShadowMapPipelineAttributes ();
	void EndRadianceInjectionPass ();

	Camera* GetLightCamera (Scene* scene, Camera* viewCamera);

	void StartRayTrace ();
	void VoxelRenderingRayTracePass ();

	void UpdateVoxelVolumeBoundingBox (Scene*);
};

#endif