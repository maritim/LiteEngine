#ifndef CONTAINERRENDERPASS_H
#define CONTAINERRENDERPASS_H

#include "ContainerRenderSubPassI.h"

#include <vector>

#include "ContainerRenderVolumeCollectionI.h"

#include "ContainerRenderPassBuilder.h"

class ContainerRenderPassBuilder;

class ENGINE_API ContainerRenderPass : public ContainerRenderSubPassI
{
	friend ContainerRenderPassBuilder;

	DECLARE_RENDER_PASS(ContainerRenderPass)

protected:
	std::vector<ContainerRenderSubPassI*> _renderSubPasses;
	ContainerRenderVolumeCollectionI* _renderVolumeCollection;

public:
	~ContainerRenderPass ();

	void Init (const RenderSettings& settings);
	RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera, 
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	bool IsAvailable (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, const RenderVolumeCollection* rvc) const;

	void Clear ();

	static ContainerRenderPassBuilder Builder ();
protected:
	ContainerRenderPass (
		const std::vector<ContainerRenderSubPassI*>& renderSubPasses, 
		ContainerRenderVolumeCollectionI* containerRenderVolumeCollection);

	RenderVolumeCollection* IterateOverSubPasses (RenderVolumeI*, const RenderScene*, 
		const Camera*, const RenderSettings&, RenderVolumeCollection*);
};

#endif