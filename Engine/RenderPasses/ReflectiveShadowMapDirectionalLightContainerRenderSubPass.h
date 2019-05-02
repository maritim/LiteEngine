#ifndef REFLECTIVESHADOWMAPDIRECTIONALLIGHTCONTAINERRENDERSUBPASS_H
#define REFLECTIVESHADOWMAPDIRECTIONALLIGHTCONTAINERRENDERSUBPASS_H

#include "RenderPasses/DirectionalVolumetricLightContainerRenderSubPass.h"
#include "Core/Observer/ObserverI.h"

#include <string>

#include "Systems/Settings/SettingsObserverArgs.h"

class ReflectiveShadowMapDirectionalLightContainerRenderSubPass : public DirectionalVolumetricLightContainerRenderSubPass, public ObserverI<SettingsObserverArgs>
{
protected:
	float _radius;
	float _intensity;
	std::string _shadowShaderName;

public:
	ReflectiveShadowMapDirectionalLightContainerRenderSubPass ();

	void Init (const RenderSettings& settings);

	void Notify (Object* sender, const SettingsObserverArgs& args);

	void Clear ();
protected:
	void DirectionalLightPass (const Scene*, const Camera*, RenderVolumeCollection*);

	void LockShader (const VolumetricLight*);

	std::vector<PipelineAttribute> GetCustomAttributes () const;

	void InitSettings ();
	void ClearSettings ();
};

#endif