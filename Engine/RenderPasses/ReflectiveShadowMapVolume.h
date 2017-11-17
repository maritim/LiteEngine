#ifndef REFLECTIVESHADOWMAPVOLUME_H
#define RELECTIVESHADOWMAPVOLUME_H

#include "Renderer/RenderVolumeI.h"
#include "Shadows/ReflectiveShadowMapDirectionalLightVolume.h"

#include "Systems/Camera/Camera.h"

class ReflectiveShadowMapVolume : public RenderVolumeI, public ReflectiveShadowMapDirectionalLightVolume
{
protected:
	Camera* _camera;

public:
	virtual void SetLightCamera (Camera*);
	virtual Camera* GetLightCamera ();

	virtual void BindForReading ();
	virtual void BindForWriting ();
	virtual std::vector<PipelineAttribute> GetCustomAttributes ();};

#endif