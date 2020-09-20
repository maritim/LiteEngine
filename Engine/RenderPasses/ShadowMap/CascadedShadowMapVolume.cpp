#include "CascadedShadowMapVolume.h"

#include "Renderer/PipelineAttribute.h"
#include "Core/Console/Console.h"

CascadedShadowMapVolume::CascadedShadowMapVolume (const Resource<Framebuffer>& framebuffer, std::size_t cascadeLevels) :
	FramebufferRenderVolume (framebuffer),
	_cascadeLevels (cascadeLevels),
	_lightCameras (),
	_shadowMapZEnd ()
{
	/*
	 * Initialize cascaded levels
	*/

	_cascadeLevels = cascadeLevels;

	/*
	 * Initialize memory
	*/

	_lightCameras.resize (_cascadeLevels, nullptr);
	_shadowMapZEnd.resize (_cascadeLevels);

	/*
	 * Create attributes
	*/

	PipelineAttribute cascadesCount;
	PipelineAttribute shadowBias;

	cascadesCount.name = "cascadesCount";
	shadowBias.name = "shadowBias";

	cascadesCount.type = PipelineAttribute::AttrType::ATTR_1I;
	shadowBias.type = PipelineAttribute::AttrType::ATTR_1F;

	cascadesCount.value.x = _cascadeLevels;

	_attributes.push_back (cascadesCount);
	_attributes.push_back (shadowBias);

	for (std::size_t index = 0; index<_cascadeLevels; index++) {

		PipelineAttribute lightSpaceMatrix;
		PipelineAttribute clipZLevel;

		lightSpaceMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;
		clipZLevel.type = PipelineAttribute::AttrType::ATTR_1F;

		lightSpaceMatrix.name = "lightSpaceMatrices[" + std::to_string (index) + "]";
		clipZLevel.name = "clipZLevels[" + std::to_string (index) + "]";

		_attributes.push_back (lightSpaceMatrix);
		_attributes.push_back (clipZLevel);
	}
}

CascadedShadowMapVolume::~CascadedShadowMapVolume ()
{
	/*
	 * Delete light cameras
	*/

	for (std::size_t index = 0; index < _cascadeLevels; index++) {
		delete _lightCameras [index];
	}

	_lightCameras.clear ();

	_shadowMapZEnd.clear ();
}

void CascadedShadowMapVolume::SetLightCamera (std::size_t cascadeLevel, Camera* camera)
{
	/*
	 * Check if cascaded level excedes the maximum level
	*/

	if (cascadeLevel >= _cascadeLevels) {
		Console::LogWarning ("There is not level " + std::to_string (cascadeLevel) + " on directional shadow map");
		return;
	}

	// delete _lightCameras [cascadedLevel];

	/*
	 * Configure light camera
	*/

	_lightCameras [cascadeLevel] = camera;

	/*
	 * Update attributes
	*/

	glm::mat4 lightProjection = _lightCameras [cascadeLevel]->GetProjectionMatrix ();
	glm::mat4 lightView = glm::translate (glm::mat4_cast(_lightCameras [cascadeLevel]->GetRotation ()), _lightCameras [cascadeLevel]->GetPosition () * -1.0f);
	glm::mat4 screenMatrix = glm::scale (glm::translate (glm::mat4 (1), glm::vec3 (0.5f)), glm::vec3 (0.5f));
	glm::mat4 cascadeMatrix = glm::scale (glm::translate (glm::mat4 (1), glm::vec3 (cascadeLevel % 2, cascadeLevel / 2, 0) * 0.5f), glm::vec3 (0.5f, 0.5f, 1.0f));

	_attributes [3 + cascadeLevel * 2].matrix = cascadeMatrix * screenMatrix * lightProjection * lightView;
}

void CascadedShadowMapVolume::SetCameraLimit (std::size_t cascadeLevel, float zLimit)
{
	/*
	 * Check if cascaded level excedes the maximum level
	*/

	if (cascadeLevel >= _cascadeLevels) {
		Console::LogWarning ("There is not level " + std::to_string (cascadeLevel) + " on directional shadow map");
		return;
	}

	/*
	 * Configure camera limit
	*/

	_shadowMapZEnd [cascadeLevel] = zLimit;

	/*
	 * Update attributes
	*/

	_attributes [3 + 1 + cascadeLevel * 2].value.x = _shadowMapZEnd [cascadeLevel];
}

void CascadedShadowMapVolume::SetShadowBias (float shadowBias)
{
	/*
	 * Update attributes
	*/

	_attributes [2].value.x = shadowBias;
}

Camera* CascadedShadowMapVolume::GetLightCamera (std::size_t cascadeLevel)
{
	/*
	 * Check if cascaded level excedes the maximum level
	*/

	if (cascadeLevel >= _cascadeLevels) {
		Console::LogWarning ("There is not level " + std::to_string (cascadeLevel) + " on directional shadow map");
		return nullptr;
	}

	return _lightCameras [cascadeLevel];
}

float CascadedShadowMapVolume::GetCameraLimit (std::size_t cascadeLevel)
{
	/*
	 * Check if cascaded level excedes the maximum level
	*/

	if (cascadeLevel >= _cascadeLevels) {
		Console::LogWarning ("There is not level " + std::to_string (cascadeLevel) + " on directional shadow map");
		return 0.0f;
	}

	return _shadowMapZEnd [cascadeLevel];
}

std::size_t CascadedShadowMapVolume::GetCascadeLevels () const
{
	return _cascadeLevels;
}
