#ifndef PIPELINE_H
#define PIPELINE_H

#include "Core/Math/Vector3.h"
#include "SceneGraph/Transform.h"
#include "Material/Material.h"
#include "Systems/Camera/Camera.h"

#include "Core/Math/glm/glm.hpp"

#include "PipelineAttribute.h"

#include "Shader/Shader.h"

// TODO: Refactor this

class Pipeline
{
private:
	static glm::mat4 _modelMatrix;
	static glm::mat4 _viewMatrix;
	static glm::mat4 _projectionMatrix;

	static glm::vec3 _cameraPosition;

	static std::size_t _textureCount;

public:
	static void SetShader (Shader* shader);

	static void SetObjectTransform (Transform *transform);
	static void SendCamera (Camera* camera);
	static void CreatePerspective (float FOV, float aspect, float zNear, float zFar);

	static void UpdateMatrices (Shader* shader);
	static void SendLights (Shader* shader);
	static void SendMaterial (Material* material);
	// TODO: Reimplement this
	static void SendCustomAttributes (const std::string& shadername, 
		const std::vector<PipelineAttribute>& attrs);
};

#endif