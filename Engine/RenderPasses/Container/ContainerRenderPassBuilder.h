#ifndef CONTAINERRENDERPASSBUILDER_H
#define CONTAINERRENDERPASSBUILDER_H

#include "Core/Interfaces/Object.h"

#include <vector>

#include "ContainerRenderPass.h"
#include "ContainerRenderSubPassI.h"
#include "ContainerRenderVolumeCollectionI.h"

class ContainerRenderPass;

class ENGINE_API ContainerRenderPassBuilder : public Object
{
protected:
	std::vector<ContainerRenderSubPassI*> _renderSubPasses;
	ContainerRenderVolumeCollectionI* _renderVolumeCollection;

public:
	ContainerRenderPassBuilder ();

	ContainerRenderPass* Build ();
	ContainerRenderPassBuilder& Attach (ContainerRenderSubPassI*);
	ContainerRenderPassBuilder& Volume (ContainerRenderVolumeCollectionI*);
};

#endif