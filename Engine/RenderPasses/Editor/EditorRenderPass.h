#ifndef EDITORRENDERPASS_H
#define EDITORRENDERPASS_H

#include "Renderer/RenderPassI.h"

class EditorRenderPass : public RenderPassI
{
protected:
	std::string _shaderName;
	unsigned int _fontTextureID;
	unsigned int _VAO_ID;
	unsigned int _VBO_ID;
	unsigned int _IBO_ID;

public:
	EditorRenderPass ();
	~EditorRenderPass ();

	virtual void Init ();
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera, RenderVolumeCollection* rvc);
protected:
	void StartEditorPass ();
	void EditorPass ();
	void EndEditorPass ();

	std::vector<PipelineAttribute> GetCustomAttributes ();

	void LoadFont ();
	void LockShader ();

	void Clear ();
};

#endif