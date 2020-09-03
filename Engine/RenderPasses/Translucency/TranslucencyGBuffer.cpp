#include "TranslucencyGBuffer.h"

std::vector<PipelineAttribute> TranslucencyGBuffer::GetCustomAttributes () const
{
    std::vector<PipelineAttribute> attributes;

    PipelineAttribute deferredTexture1;
    PipelineAttribute deferredTexture2;
    PipelineAttribute deferredTexture3;
    PipelineAttribute deferredTexture4;
    PipelineAttribute deferredTexture5;
    PipelineAttribute deferredTexture6;
    PipelineAttribute screenSize;

    deferredTexture1.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
    deferredTexture2.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
    deferredTexture3.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
    deferredTexture4.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
    deferredTexture5.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
    deferredTexture6.type = PipelineAttribute::AttrType::ATTR_TEXTURE_VIEW_DEPTH;
    screenSize.type = PipelineAttribute::AttrType::ATTR_2F;

    deferredTexture1.name = "gTrPositionMap";
    deferredTexture2.name = "gTrNormalMap";
    deferredTexture3.name = "gTrDiffuseMap";
    deferredTexture4.name = "gTrSpecularMap";
    deferredTexture5.name = "gTrEmissiveMap";
    deferredTexture6.name = "gTrDepthMap";
    screenSize.name = "screenSize";

    deferredTexture1.value.x = m_textures [0];
    deferredTexture2.value.x = m_textures [1];
    deferredTexture3.value.x = m_textures [2];
    deferredTexture4.value.x = m_textures [3];
    deferredTexture5.value.x = m_textures [4];
    deferredTexture6.value.x = _depthTextureView;
    screenSize.value = glm::vec3 (_size, 0.0f);

    attributes.push_back (deferredTexture1);
    attributes.push_back (deferredTexture2);
    attributes.push_back (deferredTexture3);
    attributes.push_back (deferredTexture4);
    attributes.push_back (deferredTexture5);
    attributes.push_back (deferredTexture6);
    attributes.push_back (screenSize);

    return attributes;
}
