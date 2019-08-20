#ifndef RSMVOLUME_H
#define RSMVOLUME_H

#include "Renderer/RenderVolumeI.h"
#include "RSMDirectionalLightVolume.h"

#include "Systems/Camera/Camera.h"

class RSMVolume : public RSMDirectionalLightVolume
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