#ifndef VOXELRADIANCEINJECTIONRENDERPASS_H
#define VOXELRADIANCEINJECTIONRENDERPASS_H

#include "Renderer/RenderPassI.h"

class VoxelRadianceInjectionRenderPass : public RenderPassI
{
public:
	virtual ~VoxelRadianceInjectionRenderPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void StartRadianceInjectionPass ();
	void RadianceInjectPass (RenderVolumeCollection*);
	void EndRadianceInjectionPass ();
};

#endif