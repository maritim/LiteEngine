#ifndef PIPELINE_H
#define PIPELINE_H

#include <glm/glm.hpp>

#include "SceneGraph/Transform.h"
#include "Material/Material.h"
#include "Systems/Camera/Camera.h"

#include "PipelineAttribute.h"

#include "Shader/Shader.h"

#include "Core/Resources/Resource.h"
#include "Renderer/RenderSettings.h"
#include "Renderer/RenderViews/MaterialView.h"
#include "Renderer/RenderViews/TextureView.h"

// TODO: Refactor this

class ENGINE_API Pipeline
{
private:
	static glm::mat4 _modelMatrix;
	static glm::mat4 _viewMatrix;
	static glm::mat4 _projectionMatrix;

	static const Camera* _currentCamera;

	static std::size_t _textureCount;

	static Shader* _lockedShader;

	static Resource<MaterialView> _defaultMaterialView;
	static Resource<TextureView> _defaultTextureView;

public:
	static void Init ();
	static void Clear ();

	static void SetShader (Shader* shader);

	static void LockShader (Shader* shader);
	static void UnlockShader ();

	static void CreateProjection (const Camera* camera);
	static void CreateProjection (glm::mat4 projectionMatrix);

	static void SetObjectTransform (const Transform *transform);
	static void SendCamera (const Camera* camera);

	static void UpdateMatrices (Shader* shader);
	static void SendLights (Shader* shader);
	static void SendMaterial (Resource<MaterialView> materialView, Shader* shader = nullptr);
	// TODO: Reimplement this
	static void SendCustomAttributes (const std::string& shadername, 
		const std::vector<PipelineAttribute>& attrs);

	static void ClearObjectTransform ();
};

#endif