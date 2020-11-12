#ifndef TRSMINDIRECTDIFFUSELIGHTRENDERPASS_H
#define TRSMINDIRECTDIFFUSELIGHTRENDERPASS_H

#include "RenderPasses/ReflectiveShadowMapping/RSMIndirectDiffuseLightRenderPass.h"

class ENGINE_API TRSMIndirectDiffuseLightRenderPass : public RSMIndirectDiffuseLightRenderPass
{
	DECLARE_RENDER_PASS(TRSMIndirectDiffuseLightRenderPass)

protected:
	std::string GetPostProcessFragmentShaderPath () const;
	FramebufferRenderVolume* CreatePostProcessVolume (const RenderSettings& settings) const;
};

#endif