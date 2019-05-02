#ifndef VOXELCONETRACEDIRECTIONALLIGHTRENDERPASS_H
#define VOXELCONETRACEDIRECTIONALLIGHTRENDERPASS_H

#include "RenderPasses/DirectionalVolumetricLightContainerRenderSubPass.h"
#include "Core/Observer/ObserverI.h"

#include <string>

#include "Systems/Settings/SettingsObserverArgs.h"

class VoxelConeTraceDirectionalLightRenderPass : public DirectionalVolumetricLightContainerRenderSubPass, public ObserverI<SettingsObserverArgs>
{
protected:
	std::string _shadowShaderName;
	float _indirectIntensity;

public:
	VoxelConeTraceDirectionalLightRenderPass ();

	void Init (const RenderSettings& settings);

	void Notify (Object* sender, const SettingsObserverArgs& args);

	void Clear ();
protected:
	void LockShader (const VolumetricLight*);

	std::vector<PipelineAttribute> GetCustomAttributes () const;

	void InitSettings ();
	void ClearSettings ();
};

#endif