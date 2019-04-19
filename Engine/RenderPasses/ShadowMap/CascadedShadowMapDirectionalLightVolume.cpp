#include "CascadedShadowMapDirectionalLightVolume.h"

#include "Wrappers/OpenGL/GL.h"

#include "Renderer/Pipeline.h"
#include "Core/Console/Console.h"

CascadedShadowMapDirectionalLightVolume::CascadedShadowMapDirectionalLightVolume () :
	_shadowMaps (),
	_shadowMapResolutions (),
	_lightCameras (),
	_shadowMapZEnd ()
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
		_shadowMapResolutions [index] = std::pair<GLuint, GLuint> (
			resolution.x, resolution.y
		);

		if (!_shadowMaps [index]->Init (
				_shadowMapResolutions [index].first,
				_shadowMapResolutions [index].second)) {
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

	GL::Viewport (0, 0, _shadowMapResolutions [cascadedLevel].first, _shadowMapResolutions [cascadedLevel].second);

	/*
	 * Bind shadow map cascade for writing
	*/

	_shadowMaps [cascadedLevel]->BindForWriting ();
}

void CascadedShadowMapDirectionalLightVolume::BindForReading ()
{
	/*
	 * Bind every depth buffer for reading
	*/

	for (std::size_t index = 0;index < _cascadedLevels;index ++) {
		GL::ActiveTexture (GL_TEXTURE4 + index);
		_shadowMaps [index]->BindForReading ();
	}
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

	cascadesCount.name = "cascadesCount";

	cascadesCount.type = PipelineAttribute::AttrType::ATTR_1I;

	cascadesCount.value.x = _cascadedLevels;

	attributes.push_back (cascadesCount);

	for (std::size_t index = 0; index<_cascadedLevels; index++) {

		PipelineAttribute shadowMap;
		PipelineAttribute lightSpaceMatrix;
		PipelineAttribute clipZLevel;

		shadowMap.type = PipelineAttribute::AttrType::ATTR_1I;
		lightSpaceMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;
		clipZLevel.type = PipelineAttribute::AttrType::ATTR_1F;

		shadowMap.name = "shadowMaps[" + std::to_string (index) + "]";
		lightSpaceMatrix.name = "lightSpaceMatrices[" + std::to_string (index) + "]";
		clipZLevel.name = "clipZLevels[" + std::to_string (index) + "]";

		shadowMap.value.x = 4.0f + index;

		glm::mat4 lightProjection = _lightCameras [index]->GetProjectionMatrix ();
		glm::mat4 lightView = glm::translate (glm::mat4_cast(_lightCameras [index]->GetRotation ()), _lightCameras [index]->GetPosition () * -1.0f);

		lightSpaceMatrix.matrix = lightProjection * lightView;

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

	/*
	 * Delete light cameras
	*/

	for (std::size_t index = 0; index < _cascadedLevels; index++) {
		delete _lightCameras [index];
	}
}
