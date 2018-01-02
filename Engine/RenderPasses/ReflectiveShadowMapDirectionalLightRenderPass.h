#ifndef REFLECTIVESHADOWMAPDIRECTIONALLIGHTRENDERPASS_H
#define REFLECTIVESHADOWMAPDIRECTIONALLIGHTRENDERPASS_H

#include "DirectionalLightRenderPassI.h"

class ReflectiveShadowMapDirectionalLightRenderPass : public DirectionalLightRenderPassI
{
protected:
	void InitContainer ();
};

#endif