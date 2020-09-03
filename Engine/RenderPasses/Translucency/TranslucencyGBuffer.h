#ifndef TRANSLUCENCYGBUFFER_H
#define TRANSLUCENCYGBUFFER_H

#include "RenderPasses/GBuffer.h"

#define TRANSLUCENCY_GBUFFER_FBO_NOT_INIT 300

class TranslucencyGBuffer : public GBuffer
{
public:
    std::vector<PipelineAttribute> GetCustomAttributes () const;
}; 

#endif