#ifndef GUIGIZMOSRENDERPASS_H
#define GUIGIZMOSRENDERPASS_H

#include "Renderer/RenderPassI.h"

#include <string>

#include "Systems/GUI/Gizmo/Gizmo.h"

class ENGINE_API GUIGizmosRenderPass : public RenderPassI
{
protected:
	std::string _shaderName;

public:
	GUIGizmosRenderPass ();

	virtual void Init (const RenderSettings& settings);
	virtual RenderVolumeCollection* Execute (const Scene* scene, const Camera* camera,
		const RenderSettings& settings, RenderVolumeCollection* rvc);

	void Clear ();
protected:
	void StartGizmoDraw (RenderVolumeCollection* rvc);

	void DrawGizmos (const Camera* camera);
	void DrawLines (const Camera* camera);

	void AddLine (std::vector<float>&, std::size_t stride, const GizmoLine& line);

	void DrawData (const std::vector<float>& data, const std::vector<unsigned int>& indices, const Camera* camera, bool depth = false);
};

#endif