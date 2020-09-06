#include "CascadedShadowMapDirectionalLightVolume.h"

#include "Wrappers/OpenGL/GL.h"

#include "Renderer/Pipeline.h"
#include "Core/Console/Console.h"

CascadedShadowMapDirectionalLightVolume::CascadedShadowMapDirectionalLightVolume () :
	_cascadedLevels (0),
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

		_shadowMapResolutions [index] = resolution;

		Resource<Texture> texture = Resource<Texture> (new Texture ("shadowMap"));

		texture->SetSize (Size (resolution.x, resolution.y));
		texture->SetMipmapGeneration (false);
		texture->SetSizedInternalFormat (TEXTURE_SIZED_INTERNAL_FORMAT::FORMAT_DEPTH16);
		texture->SetInternalFormat (TEXTURE_INTERNAL_FORMAT::FORMAT_DEPTH);
		texture->SetChannelType (TEXTURE_CHANNEL_TYPE::CHANNEL_FLOAT);
		texture->SetWrapMode (TEXTURE_WRAP_MODE::WRAP_CLAMP_BORDER);
		texture->SetMinFilter (TEXTURE_FILTER_MODE::FILTER_LINEAR);
		texture->SetMagFilter (TEXTURE_FILTER_MODE::FILTER_LINEAR);
		texture->SetAnisotropicFiltering (false);
		texture->SetBorderColor (Color (glm::vec4 (1.0)));

		Resource<Framebuffer> framebuffer = Resource<Framebuffer> (new Framebuffer (nullptr, texture));

		_shadowMaps [index] = new FramebufferRenderVolume (framebuffer);
	}

	_attributes.clear ();

	PipelineAttribute cascadesCount;
	PipelineAttribute shadowBias;

	cascadesCount.name = "cascadesCount";
	shadowBias.name = "shadowBias";

	cascadesCount.type = PipelineAttribute::AttrType::ATTR_1I;
	shadowBias.type = PipelineAttribute::AttrType::ATTR_1F;

	cascadesCount.value.x = _cascadedLevels;

	_attributes.push_back (cascadesCount);
	_attributes.push_back (shadowBias);

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

		shadowMap.value.x = _shadowMaps [index]->GetFramebufferView ()->GetDepthTextureView ()->GetGPUIndex ();

		_attributes.push_back (shadowMap);
		_attributes.push_back (lightSpaceMatrix);
		_attributes.push_back (clipZLevel);
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

	_shadowMaps [cascadedLevel]->GetFramebufferView ()->Activate ();

	GL::Clear (GL_DEPTH_BUFFER_BIT);
}

const std::vector<PipelineAttribute>& CascadedShadowMapDirectionalLightVolume::GetCustomAttributes () const
{
	return _attributes;
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

	// delete _lightCameras [cascadedLevel];

	/*
	 * Configure light camera
	*/

	_lightCameras [cascadedLevel] = camera;

	/*
	 * Update attributes
	*/

	glm::mat4 lightProjection = _lightCameras [cascadedLevel]->GetProjectionMatrix ();
	glm::mat4 lightView = glm::translate (glm::mat4_cast(_lightCameras [cascadedLevel]->GetRotation ()), _lightCameras [cascadedLevel]->GetPosition () * -1.0f);
	glm::mat4 screenMatrix = glm::scale (glm::translate (glm::mat4 (1), glm::vec3 (0.5f)), glm::vec3 (0.5f));

	_attributes [2 + 1 + cascadedLevel * 3].matrix = screenMatrix * lightProjection * lightView;
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

	/*
	 * Update attributes
	*/

	_attributes [2 + 2 + cascadedLevel * 3].value.x = _shadowMapZEnd [cascadedLevel];
}

void CascadedShadowMapDirectionalLightVolume::SetShadowBias (float shadowBias)
{
	/*
	 * Update attributes
	*/

	_attributes [1].value.x = shadowBias;
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

FramebufferRenderVolume* CascadedShadowMapDirectionalLightVolume::GetShadowMapVolume (std::size_t cascadedLevel)
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
