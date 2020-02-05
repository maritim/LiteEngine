#ifndef PIPELINE_H
#define PIPELINE_H

#include <glm/glm.hpp>

#include "SceneGraph/Transform.h"
#include "Material/Material.h"
#include "Systems/Camera/Camera.h"

#include "PipelineAttribute.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderSettings.h"
#include "Renderer/RenderViews/MaterialView.h"
#include "Renderer/RenderViews/TextureView.h"
#include "Renderer/RenderViews/ShaderView.h"

// TODO: Refactor this

class ENGINE_API Pipeline
{
private:
	static glm::mat4 _modelMatrix;
	static glm::mat4 _viewMatrix;
	static glm::mat4 _projectionMatrix;

	static const Camera* _currentCamera;

	static std::size_t _textureCount;

	static Resource<ShaderView> _lockedShaderView;

	static Resource<MaterialView> _defaultMaterialView;
	static Resource<TextureView> _defaultTextureView;

public:
	static void Init ();
	static void Clear ();

	static void SetShader (const Resource<ShaderView>& shaderView);

	static void LockShader (const Resource<ShaderView>& shaderView);
	static void UnlockShader ();

	static void CreateProjection (const Camera* camera);
	static void CreateProjection (glm::mat4 projectionMatrix);

	static void SetObjectTransform (const Transform *transform);
	static void SendCamera (const Camera* camera);

	static void UpdateMatrices (const Resource<ShaderView>& shaderView);
	static void SendLights (const Resource<ShaderView>& shaderView);
	static void SendMaterial (Resource<MaterialView> materialView, const Resource<ShaderView>& shaderView = nullptr);
	// TODO: Reimplement this
	static void SendCustomAttributes (const Resource<ShaderView>& shaderView,
		const std::vector<PipelineAttribute>& attrs);

	static void ClearObjectTransform ();
};

#endif