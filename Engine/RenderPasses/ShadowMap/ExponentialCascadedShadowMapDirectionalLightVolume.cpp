#include "ExponentialCascadedShadowMapDirectionalLightVolume.h"

#include "RenderPasses/ShadowMap/ExponentialShadowMapVolume.h"

#include "Core/Console/Console.h"

bool ExponentialCascadedShadowMapDirectionalLightVolume::Init (std::size_t cascadedLevels, const glm::ivec2& resolution)
{
	/*
	 * Initialize cascaded levels
	*/

	_cascadedLevels = cascadedLevels;

	/*
	 * Initialize shadow maps
	*/

	_shadowMaps.resize (_cascadedLevels, nullptr);
	_shadowMapResolutions.resize (_cascadedLevels);
	_lightCameras.resize (_cascadedLevels, nullptr);
	_shadowMapZEnd.resize (_cascadedLevels);

	for (std::size_t index = 0; index < _cascadedLevels; index ++) {
		_shadowMaps [index] = new ExponentialShadowMapVolume ();
	}

	for (std::size_t index = 0; index < _cascadedLevels; index ++) {
		_shadowMapResolutions [index] = std::pair<GLuint, GLuint> (
			resolution.x, resolution.y
		);

		if (!_shadowMaps [index]->Init (
				_shadowMapResolutions [index].first,
				_shadowMapResolutions [index].second)) {
			Console::LogError ("Exponential Shadow Map Frame Buffer is not complete!");
			return false;
		}
	}	

	return true;
}

void ExponentialCascadedShadowMapDirectionalLightVolume::SetExponential (float exponential)
{
	_exponential = exponential;
}

std::vector<PipelineAttribute> ExponentialCascadedShadowMapDirectionalLightVolume::GetCustomAttributes () const
{
	/*
	 * Attach cascade shadow map directional light volume attributes to pipeline
	*/

	std::vector<PipelineAttribute> attributes = CascadedShadowMapDirectionalLightVolume::GetCustomAttributes ();

	/*
	 * Attach exponential shadow map directional light volume attributes to pipeline
	*/

	PipelineAttribute exponential;

	exponential.type = PipelineAttribute::AttrType::ATTR_1F;

	exponential.name = "exponential";

	exponential.value.x = _exponential;

	attributes.push_back (exponential);

	return attributes;
}
