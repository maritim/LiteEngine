#ifndef GUIRENDERPASS_H
#define GUIRENDERPASS_H

#include "Renderer/RenderPassI.h"

class GUIRenderPass : public RenderPassI
{
protected:
	std::string _shaderName;
	unsigned int _VAO_ID;
	unsigned int _VBO_ID;
	unsigned int _IBO_ID;

public:
	GUIRenderPass ();
	~GUIRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);
protected:
	void StartEditorPass ();
	void EditorPass (const RenderSettings& settings);
	void EndEditorPass ();

	std::vector<PipelineAttribute> GetCustomAttributes ();

	void LockShader ();

	void Clear ();
};

#endif