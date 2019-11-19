#include "CascadedShadowMapDirectionalLightVolume.h"

#include "Wrappers/OpenGL/GL.h"

#include "Renderer/Pipeline.h"
#include "Core/Console/Console.h"

CascadedShadowMapDirectionalLightVolume::CascadedShadowMapDirectionalLightVolume () :
	_cascadedLevels (0),
	_shadowMaps (),
	_shadowMapResolutions (),
	_lightCameras (),
	_shadowMapZEnd (),
	_shadowBias (0.0f)
{

}

CascadedShadowMapDirectionalLightVolume::~CascadedShadowMapDirectionalLightVolume ()
{

}

bool CascadedShadowMapDirectionalLightVolume::Init (std::size_t cascadedLevels, const glm::ivec2& resolution)
{
	/*
	 * Initialize cascaded levels
	*/

	_cascadedLevels = cascadedLevels;

	/*
	 * Initialize memory
	*/

	_shadowMaps.resize (_cascadedLevels, nullptr);
	_shadowMapResolutions.resize (_cascadedLevels);
	_lightCameras.resize (_cascadedLevels, nullptr);
	_shadowMapZEnd.resize (_cascadedLevels);

	/*
	 * Create shadow maps
	*/

	for (std::size_t index = 0; index < _cascadedLevels; index ++) {
		_shadowMaps [index] = new ShadowMapVolume ();
	}

	/*
	 * Initialize shadow map
	*/

	for (std::size_t index = 0; index < _cascadedLevels; index ++) {
		_shadowMapResolutions [index] = resolution;

		if (!_shadowMaps [index]->Init (_shadowMapResolutions [index])) {
			Console::LogError ("Shadow Map Frame Buffer is not complete!");
			return false;
		}
	}

	return true;
}

void CascadedShadowMapDirectionalLightVolume::BindForShadowMapCatch (std::size_t cascadedLevel)
{
	/*
	 * Check if cascaded level excedes the maximum level
	*/

	if (cascadedLevel >= _cascadedLevels) {
		Console::LogWarning ("There is not level " + std::to_string (cascadedLevel) + " on directional shadow map");
		return;
	}

	/*
	 * Change resolution on viewport as shadow map size
	*/

	GL::Viewport (0, 0, _shadowMapResolutions [cascadedLevel].x, _shadowMapResolutions [cascadedLevel].y);

	/*
	 * Bind shadow map cascade for writing
	*/

	_shadowMaps [cascadedLevel]->BindForWriting ();
}

void CascadedShadowMapDirectionalLightVolume::BindForReading ()
{
	/*
	 * Do nothing
	*/
}

void CascadedShadowMapDirectionalLightVolume::BindForWriting ()
{
	/*
	 * Nothing to do
	*/
}

std::vector<PipelineAttribute> CascadedShadowMapDirectionalLightVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute cascadesCount;
	PipelineAttribute shadowBias;

	cascadesCount.name = "cascadesCount";
	shadowBias.name = "shadowBias";

	cascadesCount.type = PipelineAttribute::AttrType::ATTR_1I;
	shadowBias.type = PipelineAttribute::AttrType::ATTR_1F;

	cascadesCount.value.x = _cascadedLevels;
	shadowBias.value.x = _shadowBias;

	attributes.push_back (cascadesCount);
	attributes.push_back (shadowBias);

	for (std::size_t index = 0; index<_cascadedLevels; index++) {

		PipelineAttribute shadowMap;
		PipelineAttribute lightSpaceMatrix;
		PipelineAttribute clipZLevel;

		shadowMap.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
		lightSpaceMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;
		clipZLevel.type = PipelineAttribute::AttrType::ATTR_1F;

		shadowMap.name = "shadowMaps[" + std::to_string (index) + "]";
		lightSpaceMatrix.name = "lightSpaceMatrices[" + std::to_string (index) + "]";
		clipZLevel.name = "clipZLevels[" + std::to_string (index) + "]";

		shadowMap.value.x = _shadowMaps [index]->GetColorTextureID ();

		glm::mat4 lightProjection = _lightCameras [index]->GetProjectionMatrix ();
		glm::mat4 lightView = glm::translate (glm::mat4_cast(_lightCameras [index]->GetRotation ()), _lightCameras [index]->GetPosition () * -1.0f);
		glm::mat4 screenMatrix = glm::scale (glm::translate (glm::mat4 (1), glm::vec3 (0.5f)), glm::vec3 (0.5f));

		lightSpaceMatrix.matrix = screenMatrix * lightProjection * lightView;

		clipZLevel.value.x = _shadowMapZEnd [index];

		attributes.push_back (shadowMap);
		attributes.push_back (lightSpaceMatrix);
		attributes.push_back (clipZLevel);
	}

	return attributes;
}

void CascadedShadowMapDirectionalLightVolume::SetLightCamera (std::size_t cascadedLevel, Camera* camera)
{
	/*
	 * Check if cascaded level excedes the maximum level
	*/

	if (cascadedLevel >= _cascadedLevels) {
		Console::LogWarning ("There is not level " + std::to_string (cascadedLevel) + " on directional shadow map");
		return;
	}

	delete _lightCameras [cascadedLevel];

	/*
	 * Configure light camera
	*/

	_lightCameras [cascadedLevel] = camera;
}

void CascadedShadowMapDirectionalLightVolume::SetCameraLimit (std::size_t cascadedLevel, float zLimit)
{
	/*
	 * Check if cascaded level excedes the maximum level
	*/

	if (cascadedLevel >= _cascadedLevels) {
		Console::LogWarning ("There is not level " + std::to_string (cascadedLevel) + " on directional shadow map");
		return;
	}

	/*
	 * Configure camera limit
	*/

	_shadowMapZEnd [cascadedLevel] = zLimit;
}

void CascadedShadowMapDirectionalLightVolume::SetShadowBias (float shadowBias)
{
	_shadowBias = shadowBias;
}

Camera* CascadedShadowMapDirectionalLightVolume::GetLightCamera (std::size_t cascadedLevel)
{
	/*
	 * Check if cascaded level excedes the maximum level
	*/

	if (cascadedLevel >= _cascadedLevels) {
		Console::LogWarning ("There is not level " + std::to_string (cascadedLevel) + " on directional shadow map");
		return nullptr;
	}

	return _lightCameras [cascadedLevel];
}

float CascadedShadowMapDirectionalLightVolume::GetCameraLimit (std::size_t cascadedLevel)
{
	/*
	 * Check if cascaded level excedes the maximum level
	*/

	if (cascadedLevel >= _cascadedLevels) {
		Console::LogWarning ("There is not level " + std::to_string (cascadedLevel) + " on directional shadow map");
		return 0.0f;
	}

	return _shadowMapZEnd [cascadedLevel];
}

ShadowMapVolume* CascadedShadowMapDirectionalLightVolume::GetShadowMapVolume (std::size_t cascadedLevel)
{
	/*
	 * Check if cascaded level excedes the maximum level
	*/

	if (cascadedLevel >= _cascadedLevels) {
		Console::LogWarning ("There is not level " + std::to_string (cascadedLevel) + " on directional shadow map");
		return nullptr;
	}

	return _shadowMaps [cascadedLevel];
}

std::size_t CascadedShadowMapDirectionalLightVolume::GetCascadesCount () const
{
	return _cascadedLevels;
}

void CascadedShadowMapDirectionalLightVolume::Clear ()
{
	/*
	 * Clear every shadow map
	*/

	for (std::size_t index = 0; index < _cascadedLevels; index ++) {
		_shadowMaps [index]->Clear ();
	}

	/*
	 * Delete shadow maps
	*/

	for (std::size_t index = 0; index < _cascadedLevels; index ++) {
		delete _shadowMaps [index];
	}

	_shadowMaps.clear ();

	/*
	 * Delete light cameras
	*/

	for (std::size_t index = 0; index < _cascadedLevels; index++) {
		delete _lightCameras [index];
	}

	_lightCameras.clear ();

	_shadowMapResolutions.clear ();
	_shadowMapZEnd.clear ();
}
