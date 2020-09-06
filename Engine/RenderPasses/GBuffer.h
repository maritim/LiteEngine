#ifndef GBUFFER_H
#define GBUFFER_H

#include "RenderPasses/FramebufferRenderVolume.h"

class GBuffer : public FramebufferRenderVolume
{
protected:
    glm::mat4 _projectionMatrix;
    glm::vec2 _frustumJitter;

public:
    GBuffer (const Resource<Framebuffer>& framebuffer);

    void SetProjectionMatrix (const glm::mat4& projectionMatrix);
    void SetFrustumJitter (const glm::vec2& frustumJitter);

    const glm::mat4& GetProjectionMatrix () const;
    const glm::vec2& GetFrustumJitter () const;
}; 

#endif