#ifndef RSMDIRECTIONALLIGHTVOLUME_H
#define RSMDIRECTIONALLIGHTVOLUME_H

#include "RenderPasses/ShadowMap/ShadowMapVolumeI.h"

#include "Wrappers/OpenGL/GL.h"

#include "RSMBuffer.h"

#define REFLECTIVE_SHADOW_MAP_FBO_NOT_INIT 350

class RSMDirectionalLightVolume : public ShadowMapVolumeI
{
protected:
	glm::ivec2 _size;
	std::size_t _cascadedLevels;

	RSMBuffer** _shadowMapBuffers;

public:
	RSMDirectionalLightVolume ();
	~RSMDirectionalLightVolume ();

	virtual bool Init (const glm::ivec2& size, std::size_t cascadedLevels);

	bool Init (std::size_t cascadedLevels);
	void BindForShadowMapCatch (std::size_t cascadedLevel);
	void EndDrawing ();

	void BindForReading ();

	RSMBuffer* GetCascade (std::size_t cascadedLevel) const;
	glm::ivec2 GetSize () const;

	void Clear ();
};

#endif