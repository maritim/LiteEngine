#ifndef CONTAINERRENDERVOLUMECOLLECTIONI_H
#define CONTAINERRENDERVOLUMECOLLECTIONI_H

#include "Core/Interfaces/Object.h"

#include "Renderer/RenderVolumeI.h"
#include "Renderer/RenderScene.h"

class ContainerRenderVolumeCollectionI : public Object
{
public:
	virtual ~ContainerRenderVolumeCollectionI () = 0;

	virtual void Reset () = 0;
	virtual RenderVolumeI* GetNextVolume (const RenderScene*) = 0;
};

#endif