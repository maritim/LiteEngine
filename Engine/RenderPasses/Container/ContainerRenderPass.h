#ifndef CONTAINERRENDERPASS_H
#define CONTAINERRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include <vector>

#include "ContainerRenderSubPassI.h"
#include "ContainerRenderVolumeCollectionI.h"

#include "ContainerRenderPassBuilder.h"

class ContainerRenderPassBuilder;

class ContainerRenderPass : public RenderPassI
{
	friend ContainerRenderPassBuilder;

protected:
	std::vector<ContainerRenderSubPassI*> _renderSubPasses;
	ContainerRenderVolumeCollectionI* _renderVolumeCollection;

	static ContainerRenderPassBuilder* _builder;

public:
	~ContainerRenderPass ();

	void Init ();
	RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, 
		RenderVolumeCollection* rvc);

	void Clear ();

	static ContainerRenderPassBuilder& Builder ();
protected:
	ContainerRenderPass (
		const std::vector<ContainerRenderSubPassI*>& renderSubPasses, 
		ContainerRenderVolumeCollectionI* containerRenderVolumeCollection);

	RenderVolumeCollection* IterateOverSubPasses (RenderVolumeI*, const Scene*, 
		const Camera*, RenderVolumeCollection*);
};

#endif