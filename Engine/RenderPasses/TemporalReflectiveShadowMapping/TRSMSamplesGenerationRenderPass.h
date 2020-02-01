#ifndef TRSMSAMPLESGENERATIONRENDERPASS_H
#define TRSMSAMPLESGENERATIONRENDERPASS_H

#include "RenderPasses/ReflectiveShadowMapping/RSMSamplesGenerationRenderPass.h"

class TRSMSamplesGenerationRenderPass : public RSMSamplesGenerationRenderPass
{
	DECLARE_RENDER_PASS(TRSMSamplesGenerationRenderPass)

public:
	TRSMSamplesGenerationRenderPass ();
};

#endif
