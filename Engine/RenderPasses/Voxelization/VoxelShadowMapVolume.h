#ifndef VOXELSHADOWMAPVOLUME_H
#define VOXELSHADOWMAPVOLUME_H

#include "Renderer/RenderVolumeI.h"
#include "RenderPasses/ShadowMap/CascadedShadowMapDirectionalLightVolume.h"

#include "Systems/Camera/Camera.h"

class VoxelShadowMapVolume : public CascadedShadowMapDirectionalLightVolume
{
protected:
	Camera* _camera;

public:
	virtual void SetLightCamera (Camera*);
	virtual Camera* GetLightCamera ();

	virtual void BindForReading ();
	virtual void BindForWriting ();
	virtual std::vector<PipelineAttribute> GetCustomAttributes () const;
};

#endif