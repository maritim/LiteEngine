#include "ContainerRenderPassBuilder.h"

ContainerRenderPassBuilder::ContainerRenderPassBuilder () :
	_renderSubPasses (),
	_renderVolumeCollection (nullptr)
{

}

ContainerRenderPass* ContainerRenderPassBuilder::Build ()
{
	ContainerRenderPass* containerRenderPass = new ContainerRenderPass (
		_renderSubPasses, _renderVolumeCollection
	);

	_renderSubPasses.clear ();
	_renderVolumeCollection = nullptr;

	return containerRenderPass;
}

ContainerRenderPassBuilder& ContainerRenderPassBuilder::Attach (ContainerRenderSubPassI* renderSubPass)
{
	_renderSubPasses.push_back (renderSubPass);

	return *this;
}

ContainerRenderPassBuilder& ContainerRenderPassBuilder::Volume (ContainerRenderVolumeCollectionI* volumeCollection)
{
	_renderVolumeCollection = volumeCollection;

	return *this;
}