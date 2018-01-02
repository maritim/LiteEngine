#ifndef DEFERREDDIRETIONALLIGHTRENDERPASS_H
#define DEFERREDDIRETIONALLIGHTRENDERPASS_H

#include "DirectionalLightRenderPassI.h"

class DeferredDirectionalLightRenderPass : public DirectionalLightRenderPassI
{
protected:
	void InitContainer ();
};

#endif