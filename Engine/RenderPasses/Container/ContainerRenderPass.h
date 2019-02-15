#ifndef CONTAINERRENDERPASS_H
#define CONTAINERRENDERPASS_H

#include "ContainerRenderSubPassI.h"

#include <vector>

#include "ContainerRenderVolumeCollectionI.h"

#include "ContainerRenderPassBuilder.h"

class ContainerRenderPassBuilder;

class ContainerRenderPass : public ContainerRenderSubPassI
{
	friend ContainerRenderPassBuilder;

protected:
	std::vector<ContainerRenderSubPassI*> _renderSubPasses;
	ContainerRenderVolumeCollectionI* _renderVolumeCollection;

public:
	~ContainerRenderPass ();

	void Init ();
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, 
		RenderVolumeCollection* rvc);

	bool IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const;

	void Clear ();

	static ContainerRenderPassBuilder Builder ();
protected:
	ContainerRenderPass (
		const std::vector<ContainerRenderSubPassI*>& renderSubPasses, 
		ContainerRenderVolumeCollectionI* containerRenderVolumeCollection);

	RenderVolumeCollection* IterateOverSubPasses (RenderVolumeI*, const Scene*, 
		const Camera*, RenderVolumeCollection*);
};

#endif