#ifndef DEFERREDAMBIENTLIGHTRENDERCONTAINERSUBPASS_H
#define DEFERREDAMBIENTLIGHTRENDERCONTAINERSUBPASS_H

#include "RenderPasses/Container/ContainerRenderSubPassI.h"
#include "Core/Observer/ObserverI.h"

#include <string>

#include "Systems/Settings/SettingsObserverArgs.h"

class DeferredAmbientLightContainerRenderSubPass : public ContainerRenderSubPassI, public ObserverI<SettingsObserverArgs>
{
protected:
	bool _aoEnabled;
	std::string _shaderName;
	std::string _aoShaderName;

public:
	DeferredAmbientLightContainerRenderSubPass ();
	virtual ~DeferredAmbientLightContainerRenderSubPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);

	virtual bool IsAvailable (const Scene* scene, const Camera* camera, const RenderVolumeCollection* rvc) const;

	void Notify (Object* sender, const SettingsObserverArgs& args);
protected:
	void StartAmbientLightPass (RenderVolumeCollection* rvc);
	void AmbientLightPass (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);
	void EndAmbientLightPass ();

	void LockShader ();
	std::vector<PipelineAttribute> GetCustomAttributes (RenderVolumeCollection* rvc);

	void InitSettings ();
	void ClearSettings ();
};

#endif