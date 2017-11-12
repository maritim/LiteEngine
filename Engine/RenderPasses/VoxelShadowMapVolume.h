#ifndef VOXELSHADOWMAPVOLUME_H
#define VOXELSHADOWMAPVOLUME_H

#include "Renderer/RenderVolumeI.h"
#include "Shadows/ShadowMapDirectionalLightVolume.h"

#include "Systems/Camera/Camera.h"

class VoxelShadowMapVolume : public RenderVolumeI, public ShadowMapDirectionalLightVolume
{
protected:
	Camera* _camera;

public:
	virtual void SetLightCamera (Camera*);
	virtual Camera* GetLightCamera ();

	virtual void BindForReading ();
	virtual void BindForWriting ();
	virtual std::vector<PipelineAttribute> GetCustomAttributes ();
};

#endif