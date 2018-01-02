#ifndef RENDERCONTAINERPASS_H
#define RENDERCONTAINERPASS_H

#include "Renderer/RenderPassI.h"

#include <vector>

#include "RenderContainerSubPassI.h"

class RenderContainerPassI : public RenderPassI
{
protected:
	std::vector<RenderContainerSubPassI*> _renderSubPasses;

public:
	RenderContainerPassI ();
	~RenderContainerPassI ();

	void Init ();
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, 
		RenderVolumeCollection* rvc);
protected:
	virtual void InitContainer () = 0;
	virtual RenderVolumeI* NextVolume () = 0;

	RenderVolumeCollection* IterateOverSubPasses (RenderVolumeI*, const Scene*, 
		const Camera*, RenderVolumeCollection*);
};

#endif