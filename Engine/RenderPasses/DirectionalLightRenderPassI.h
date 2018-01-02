#ifndef DIRETIONALLIGHTRENDERPASSI_H
#define DIRETIONALLIGHTRENDERPASSI_H

#include "RenderContainerPassI.h"

#include "VolumetricLightVolume.h"

class DirectionalLightRenderPassI : public RenderContainerPassI
{
protected:
	VolumetricLightVolume* _volume;
	std::size_t _lightsIterator;

public:
	DirectionalLightRenderPassI ();
	~DirectionalLightRenderPassI ();
protected:
	RenderVolumeI* NextVolume ();
};

#endif