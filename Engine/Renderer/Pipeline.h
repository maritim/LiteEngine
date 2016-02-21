#ifndef PIPELINE_H
#define PIPELINE_H

#include "Core/Math/Vector3.h"
#include "Core/Math/Matrix.h"
#include "SceneGraph/Transform.h"
#include "Material/Material.h"

#include "PipelineAttribute.h"

// TODO: Refactor this

class Pipeline
{
private:
	static Matrix _modelMatrix;
	static Matrix _viewMatrix;
	static Matrix _projectionMatrix;

	static Vector3 _cameraPosition;

	static std::size_t _textureCount;

public:
	static void SetShader (unsigned int shader);

	static void SetCameraPosition (Vector3 position);
	static void SetCameraRotation (Vector3 eulerAngle);
	static void SetObjectTransform (Transform *transform);
	static void CreatePerspective (float FOV, float aspect, float zNear, float zFar);

	static void UpdateMatrices (unsigned int program);
	static void SendLights (unsigned int program);
	static void SendMaterial (Material* material);
	// TODO: Reimplement this
	static void SendCustomAttributes (const std::string& shadername, 
		const std::vector<PipelineAttribute>& attrs);
};

#endif