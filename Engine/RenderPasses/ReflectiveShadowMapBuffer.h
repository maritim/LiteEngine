#ifndef REFLECTIVESHADOWMAPBUFFER_H
#define REFLECTIVESHADOWMAPBUFFER_H

#include "RenderPasses/MultipleRenderTargetsVolume.h"

class ReflectiveShadowMapBuffer : public MultipleRenderTargetsVolume
{
protected:
    enum RSMBUFFER_TEXTURE_TYPE {
		RSMBUFFER_TEXTURE_TYPE_DEPTH,
        RSMBUFFER_TEXTURE_TYPE_POSITION,
        RSMBUFFER_TEXTURE_TYPE_NORMAL,
        RSMBUFFER_TEXTURE_TYPE_FLUX,
        RSMBUFFER_NUM_TEXTURES
    };

public:
	ReflectiveShadowMapBuffer ();
	
	bool Init(const glm::ivec2& size);

	void BindForReading ();
};

#endif
