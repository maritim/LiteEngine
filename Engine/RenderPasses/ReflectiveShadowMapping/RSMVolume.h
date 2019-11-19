#ifndef RSMDIRECTIONALLIGHTVOLUME_H
#define RSMDIRECTIONALLIGHTVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include "RSMBuffer.h"

#include "Systems/Camera/Camera.h"

#define REFLECTIVE_SHADOW_MAP_FBO_NOT_INIT 350

class RSMVolume : public RenderVolumeI
{
protected:
	glm::ivec2 _size;
	RSMBuffer* _shadowMapBuffer;

	Camera* _camera;
	float _shadowBias;

public:
	RSMVolume ();
	~RSMVolume ();

	virtual bool Init (const glm::ivec2& size);

	bool Init ();
	void BindForWriting ();
	void EndDrawing ();

	void BindForReading ();

	std::vector<PipelineAttribute> GetCustomAttributes () const;


	void SetLightCamera (Camera* camera);
	void SetShadowBias (float shadowBias);

	Camera* GetLightCamera ();
	RSMBuffer* GetShadowMapBuffer () const;
	glm::ivec2 GetSize () const;

	void Clear ();
};

#endif