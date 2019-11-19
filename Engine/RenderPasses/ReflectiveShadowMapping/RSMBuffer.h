#ifndef RSMBUFFER_H
#define RSMBUFFER_H

#include "RenderPasses/MultipleRenderTargetsVolume.h"

class RSMBuffer : public MultipleRenderTargetsVolume
{
protected:
    enum RSMBUFFER_TEXTURE_TYPE {
        RSMBUFFER_TEXTURE_TYPE_POSITION,
        RSMBUFFER_TEXTURE_TYPE_NORMAL,
        RSMBUFFER_TEXTURE_TYPE_FLUX,
        RSMBUFFER_NUM_TEXTURES
    };

public:
	RSMBuffer ();
	
	bool Init(const glm::ivec2& size);
};

#endif
