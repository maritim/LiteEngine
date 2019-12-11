#ifndef GBUFFER_H
#define GBUFFER_H

#include "RenderPasses/MultipleRenderTargetsVolume.h"

#define GBUFFER_FBO_NOT_INIT 300

class GBuffer : public MultipleRenderTargetsVolume
{
public:
    enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_NORMAL,
        GBUFFER_TEXTURE_TYPE_DIFFUSE,
        GBUFFER_TEXTURE_TYPE_SPECULAR,
        GBUFFER_TEXTURE_TYPE_EMISSIVE,
        GBUFFER_TEXTURE_TYPE_LIGHTMAP,
        GBUFFER_NUM_TEXTURES
    };
protected:
    unsigned int _depthTextureView;
    glm::mat4 _projectionMatrix;
    glm::vec2 _frustumJitter;

public:
    GBuffer();

    bool Init (const glm::ivec2& size);

    void SetProjectionMatrix (const glm::mat4& projectionMatrix);
    void SetFrustumJitter (const glm::vec2& frustumJitter);

    const glm::mat4& GetProjectionMatrix () const;
    const glm::vec2& GetFrustumJitter () const;

    std::vector<PipelineAttribute> GetCustomAttributes () const;
}; 

#endif