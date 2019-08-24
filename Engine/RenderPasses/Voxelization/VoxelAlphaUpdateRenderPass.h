#ifndef VOXELALPHAUPDATERENDERPASS_H
#define VOXELALPHAUPDATERENDERPASS_H

#include "Renderer/RenderPassI.h"

class VoxelAlphaUpdateRenderPass : public RenderPassI
{
	DECLARE_RENDER_PASS(VoxelAlphaUpdateRenderPass)

protected:
	std::string _shaderName;
	bool _firstTime;

public:
	VoxelAlphaUpdateRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const RenderScene* renderScene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void StartVoxelAlphaUpdate ();
	void UpdateVoxelAlpha (const RenderSettings&, RenderVolumeCollection*);
	void EndVoxelAlphaUpdate ();
};

#endif