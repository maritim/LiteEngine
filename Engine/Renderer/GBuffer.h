#ifndef GBUFFER_H
#define GBUFFER_H

#include <GL/glew.h>
#include <cstddef>

class GBuffer
{
public:
    enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_NORMAL,
        GBUFFER_TEXTURE_TYPE_DIFFUSE,
        GBUFFER_TEXTURE_TYPE_SPECULAR,
        GBUFFER_NUM_TEXTURES
    };

private:
    GLuint m_fbo;
    GLuint m_textures[GBUFFER_NUM_TEXTURES];
    GLuint m_depthTexture;
    GLuint m_finalTexture;

public:
    GBuffer();
    ~GBuffer();

    bool Init(std::size_t screenWidth, std::size_t screenHeight);
    bool Clear ();

    void StartFrame();
    void BindForGeomPass();
    void BindForStencilPass();
    void BindForLightPass();
    void BindForFinalPass();
}; 

#endif