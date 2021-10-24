#include "Pipeline.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer/Render/Texture/Texture.h"
#include "SceneGraph/Transform.h"
#include "Renderer/Render/Material/Material.h"
#include "Systems/Settings/SettingsManager.h"

#include "Resources/Resources.h"

#include "PipelineAttribute.h"

#include "Renderer/RenderSystem.h"

#include "Wrappers/OpenGL/GL.h"

glm::mat4 Pipeline::_modelMatrix (0);
glm::mat4 Pipeline::_viewMatrix (0);
glm::mat4 Pipeline::_projectionMatrix (0);
const Camera* Pipeline::_currentCamera (nullptr);
std::size_t Pipeline::_textureCount (0);

Resource<ShaderView> Pipeline::_lockedShaderView (nullptr);

Resource<MaterialView> Pipeline::_defaultMaterialView (nullptr);
Resource<TextureView> Pipeline::_defaultTextureView (nullptr);

void Pipeline::Init ()
{
	/*
	 * Load default material
	*/

	std::string defaultMaterialPath = "Assets/Materials/default.mtl";
	Resource<MaterialLibrary> materialLibrary = Resources::LoadMaterialLibrary (defaultMaterialPath);

	Resource<Material> defaultMaterial = materialLibrary->GetMaterial (0);

	_defaultMaterialView = RenderSystem::LoadMaterial (defaultMaterial);

	/*
	 * Load default texture
	*/

	// TODO: Extend this to default textures for every type (ambient, diffuse ...)

	std::string defaultTexturePath = "Assets/Textures/AmbientDefault.png";
	Resource<Texture> defaultTexture = Resources::LoadTexture (defaultTexturePath);

	_defaultTextureView = RenderSystem::LoadTexture (defaultTexture);
}

void Pipeline::Clear ()
{
	_defaultMaterialView = nullptr;
	_defaultTextureView = nullptr;
}

void Pipeline::SetShader (const Resource<ShaderView>& shaderView)
{
	if (_lockedShaderView != nullptr) {
		return;
	}

	_textureCount = 0;

	GL::UseProgram (shaderView->GetProgram ());
}

void Pipeline::LockShader (const Resource<ShaderView>& shaderView)
{
	if (_lockedShaderView != nullptr) {
		return;
	}

	SetShader (shaderView);
	_lockedShaderView = shaderView;
}

void Pipeline::UnlockShader ()
{
	_lockedShaderView = nullptr;
}

void Pipeline::CreateProjection (const Camera* camera)
{
	CreateProjection (camera->GetProjectionMatrix ());
}

void Pipeline::CreateProjection (glm::mat4 projectionMatrix)
{
	_projectionMatrix = projectionMatrix;
}

void Pipeline::SendCamera (const Camera* camera)
{
	_currentCamera = camera;

	_viewMatrix = glm::mat4_cast (camera->GetRotation ());

	_viewMatrix =  glm::translate (_viewMatrix, _currentCamera->GetPosition () * -1.0f);
}

// void Pipeline::SendViewport (const Viewport& viewport)
// {
// 	_currentViewport = viewport;

// 	GL::Viewport (_currentViewport.x, _currentViewport.y,
// 		_currentViewport.width, _currentViewport.height);
// }

void Pipeline::SetObjectTransform (const Transform* transform)
{
	_modelMatrix = transform->GetModelMatrix ();
}

void Pipeline::ClearObjectTransform ()
{
	_modelMatrix = glm::mat4 (1.0);
}

void Pipeline::UpdateMatrices (const Resource<ShaderView>& shaderView)
{
	auto currentShaderView = shaderView;

	if (_lockedShaderView != nullptr) {
		currentShaderView = _lockedShaderView;
	}

	glm::mat4 modelViewMatrix = _viewMatrix * _modelMatrix;
	glm::mat4 viewProjectionMatrix = _projectionMatrix * _viewMatrix;
	glm::mat4 modelViewProjectionMatrix = _projectionMatrix * _viewMatrix * _modelMatrix;

	glm::mat3 normalWorldMatrix = glm::transpose (glm::inverse (glm::mat3 (modelViewMatrix)));
	glm::mat3 normalMatrix = glm::transpose (glm::inverse (glm::mat3 (_modelMatrix)));

	glm::mat4 inverseViewMatrix = glm::inverse (_viewMatrix);
	glm::mat4 inverseViewProjectionMatrix = glm::inverse (modelViewProjectionMatrix);
	glm::mat3 inverseNormalWorldMatrix = glm::inverse (normalWorldMatrix);

	GL::UniformMatrix4fv (currentShaderView->GetUniformLocation ("modelMatrix"), 1, GL_FALSE, glm::value_ptr (_modelMatrix));
	GL::UniformMatrix4fv (currentShaderView->GetUniformLocation ("viewMatrix"), 1, GL_FALSE, glm::value_ptr (_viewMatrix));
	GL::UniformMatrix4fv (currentShaderView->GetUniformLocation ("modelViewMatrix"), 1, GL_FALSE, glm::value_ptr (modelViewMatrix));
	GL::UniformMatrix4fv (currentShaderView->GetUniformLocation ("projectionMatrix"), 1, GL_FALSE, glm::value_ptr (_projectionMatrix));
	GL::UniformMatrix4fv (currentShaderView->GetUniformLocation ("viewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr (viewProjectionMatrix));
	GL::UniformMatrix4fv (currentShaderView->GetUniformLocation ("modelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr (modelViewProjectionMatrix));
	GL::UniformMatrix3fv (currentShaderView->GetUniformLocation ("normalMatrix"), 1, GL_FALSE, glm::value_ptr (normalMatrix));
	GL::UniformMatrix3fv (currentShaderView->GetUniformLocation ("normalWorldMatrix"), 1, GL_FALSE, glm::value_ptr (normalWorldMatrix));
	GL::UniformMatrix4fv (currentShaderView->GetUniformLocation ("inverseViewMatrix"), 1, GL_FALSE, glm::value_ptr (inverseViewMatrix));
	GL::UniformMatrix4fv (currentShaderView->GetUniformLocation ("inverseViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr (inverseViewProjectionMatrix));
	GL::UniformMatrix3fv (currentShaderView->GetUniformLocation ("inverseNormalWorldMatrix"), 1, GL_FALSE, glm::value_ptr (inverseNormalWorldMatrix));

	GL::Uniform3fv (currentShaderView->GetUniformLocation ("cameraPosition"), 1, glm::value_ptr (_currentCamera->GetPosition ()));
	GL::Uniform2f (currentShaderView->GetUniformLocation ("cameraZLimits"), _currentCamera->GetZNear (), _currentCamera->GetZFar ());

	// TODO: Change this
	bool gammaCorrectionEnabled = SettingsManager::Instance ()->GetValue<bool> ("gamma_correction", false);

	GL::Uniform1f (currentShaderView->GetUniformLocation ("gamma"), gammaCorrectionEnabled ? 2.2f : 1.0f);

	// SendLights (shader);
}

void Pipeline::SendLights (const Resource<ShaderView>& shaderView)
{
	// if (_lockedShaderView != nullptr) {
	// 	shaderView = _lockedShaderView;
	// }

	// Color ambientColor = LightsManager::Instance ()->GetAmbientColorLight ();

	// GL::Uniform3fv(shader->GetUniformLocation ("sceneAmbient"), 1, glm::value_ptr (ambientColor.ToVector3 ()));

	// const int lightsLimit = 3;

	// int spotLightCount = 0;
	// int pointLightCount = 0;
	// int directionalLightCount = 0;

	// for (std::size_t i=0;i<LightsManager::Instance ()->Size ();i++) {
	// 	Light* light = LightsManager::Instance ()->GetLight (i);
	// 	std::string address;

	// 	glm::vec4 vec4 (vec.x, vec.y, vec.z, 1.0);

	// 	switch (light->GetType ()) {
	// 		case Light::Type::DIRECTIONAL_LIGHT :
	// 			if (directionalLightCount >= lightsLimit) {
	// 				break;
	// 			}

	// 			vec = light->GetTransform ()->GetPosition ();
	// 			//vec4 = glm::vec4 (vec.x, vec.y, vec.z, 0.0);
	// 			address = "directionalLights[" + std::to_string (directionalLightCount) + "].position";
	// 			glUniform4f (glGetUniformLocation (shader->GetProgram (), address.c_str ()), vec.x, vec.y, vec.z, 0.0);

	// 			vec = light->GetColor ().ToVector3 ();
	// 			address = "directionalLights[" + std::to_string (directionalLightCount) + "].diffuse";
	// 			glUniform4f (shader->GetUniformLocation (address.c_str ()), vec.x, vec.y, vec.z, 1.0);

	// 			vec = light->GetSpecularColor ().ToVector3 ();
	// 			address = "directionalLights[" + std::to_string (directionalLightCount) + "].specular";
	// 			glUniform4f (shader->GetUniformLocation (address.c_str ()), vec.x, vec.y, vec.z, 1.0);

	// 			++ directionalLightCount;

	// 			break;
	// 		case Light::Type::POINT_LIGHT :
	// 			if (pointLightCount >= lightsLimit) {
	// 				break;
	// 			}

	// 			vec = light->GetTransform ()->GetPosition ();
	// 			vec4 = glm::vec4 (vec.x, vec.y, vec.z, 1.0);
	// 			address = "pointLights[" + std::to_string (pointLightCount) + "].position";
	// 			glUniform4fv (shader->GetUniformLocation (address.c_str ()), 1, glm::value_ptr (vec4));

	// 			vec = light->GetColor ().ToVector3 ();
	// 			address = "pointLights[" + std::to_string (pointLightCount) + "].diffuse";
	// 			glUniform4f (shader->GetUniformLocation (address.c_str ()), vec.x, vec.y, vec.z, 1.0);

	// 			vec = light->GetColor ().ToVector3 ();
	// 			address = "pointLights[" + std::to_string (pointLightCount) + "].specular";
	// 			glUniform4f (shader->GetUniformLocation (address.c_str ()), vec.x, vec.y, vec.z, 1.0);

	// 			address = "pointLights[" + std::to_string (pointLightCount) + "].constantAttenuation";
	// 			glUniform1f (shader->GetUniformLocation (address.c_str ()), light->GetConstantAttenuation ());

	// 			address = "pointLights[" + std::to_string (pointLightCount) + "].linearAttenuation";
	// 			glUniform1f (shader->GetUniformLocation (address.c_str ()), light->GetLinearAttenuation ());

	// 			address = "pointLights[" + std::to_string (pointLightCount) + "].quadraticAttenuation";
	// 			glUniform1f (shader->GetUniformLocation (address.c_str ()), light->GetQuadraticAttenuation ());

	// 			++ pointLightCount;

	// 			break;
	// 		case Light::Type::SPOT_LIGHT :
	// 			if (spotLightCount >= lightsLimit) {
	// 				break;
	// 			}

	// 			vec = light->GetTransform ()->GetPosition ();
	// 			address = "spotLights[" + std::to_string (spotLightCount) + "].position";
	// 			glUniform4f (shader->GetUniformLocation (address.c_str ()), vec.x, vec.y, vec.z, 1.0);

	// 			vec = light->GetColor ().ToVector3 ();
	// 			address = "spotLights[" + std::to_string (spotLightCount) + "].diffuse";
	// 			glUniform4f (shader->GetUniformLocation (address.c_str ()), vec.x, vec.y, vec.z, 1.0);

	// 			vec = light->GetColor ().ToVector3 ();
	// 			address = "spotLights[" + std::to_string (spotLightCount) + "].specular";
	// 			glUniform4f (shader->GetUniformLocation (address.c_str ()), vec.x, vec.y, vec.z, 1.0);

	// 			address = "spotLights[" + std::to_string (spotLightCount) + "].constantAttenuation";
	// 			glUniform1f (shader->GetUniformLocation (address.c_str ()), light->GetConstantAttenuation ());

	// 			address = "spotLights[" + std::to_string (spotLightCount) + "].linearAttenuation";
	// 			glUniform1f (shader->GetUniformLocation (address.c_str ()), light->GetLinearAttenuation ());

	// 			address = "spotLights[" + std::to_string (spotLightCount) + "].quadraticAttenuation";
	// 			glUniform1f (shader->GetUniformLocation (address.c_str ()), light->GetQuadraticAttenuation ());

	// 			address = "spotLights[" + std::to_string (spotLightCount) + "].spotCutoff";
	// 			glUniform1f (shader->GetUniformLocation (address.c_str ()), light->GetSpotCutoff ());

	// 			address = "spotLights[" + std::to_string (spotLightCount) + "].spotExponent";
	// 			glUniform1f (shader->GetUniformLocation (address.c_str ()), light->GetSpotExponent ());

	// 			vec = light->GetSpotDirection ();
	// 			address = "spotLights[" + std::to_string (spotLightCount) + "].spotDirection";
	// 			glUniform3f (shader->GetUniformLocation (address.c_str ()), vec.x, vec.y, vec.z);

	// 			++ spotLightCount;

	// 			break;
	// 	}
	// }

	// glUniform1i (shader->GetUniformLocation ("spotLightCount"), spotLightCount);
	// glUniform1i (shader->GetUniformLocation ("pointLightCount"), pointLightCount);
	// glUniform1i (shader->GetUniformLocation ("directionalLightCount"), directionalLightCount);
}

void Pipeline::SendCustomAttributes (const Resource<ShaderView>& shaderView, const std::vector<PipelineAttribute>& attr)
{
	auto currentShaderView = shaderView;

	if (_lockedShaderView != nullptr) {
		currentShaderView = _lockedShaderView;
	}

	for (std::size_t i=0;i<attr.size ();i++) {

		int unifLoc = currentShaderView->GetUniformLocation (attr [i].name.c_str ());

		switch (attr [i].type) {
			case PipelineAttribute::ATTR_1I :
				GL::Uniform1i (unifLoc, (int) attr [i].value.x);
				break;
			case PipelineAttribute::ATTR_2I :
				GL::Uniform2i (unifLoc, (int) attr [i].value.x,
					(int) attr [i].value.y);
				break;
			case PipelineAttribute::ATTR_3I :
				GL::Uniform3i (unifLoc, (int) attr [i].value.x,
					(int) attr [i].value.y, (int) attr [i].value.z);
				break;
			case PipelineAttribute::ATTR_1F :
				GL::Uniform1f (unifLoc, attr [i].value.x);
				break;
			case PipelineAttribute::ATTR_2F :
				GL::Uniform2f (unifLoc, attr [i].value.x,
					attr [i].value.y);
				break;
			case PipelineAttribute::ATTR_3F :
				GL::Uniform3f (unifLoc, attr [i].value.x,
					attr [i].value.y, attr [i].value.z);
				break;
			case PipelineAttribute::ATTR_TEXTURE_2D :	{
					GL::ActiveTexture (GL_TEXTURE0 + _textureCount);
					GL::BindTexture (GL_TEXTURE_2D, (unsigned int) attr [i].value.x);
					GL::Uniform1i (unifLoc, _textureCount);
					++ _textureCount;
				}
				break;
			case PipelineAttribute::ATTR_TEXTURE_3D : {
					GL::ActiveTexture (GL_TEXTURE0 + _textureCount);
					GL::BindTexture (GL_TEXTURE_3D, (unsigned int) attr [i].value.x);
					GL::Uniform1i (unifLoc, _textureCount);
					++ _textureCount;
				}
				break;
			case PipelineAttribute::ATTR_TEXTURE_CUBE : {
					GL::ActiveTexture (GL_TEXTURE0 + _textureCount);
					GL::BindTexture (GL_TEXTURE_CUBE_MAP, (unsigned int) attr [i].value.x);
					GL::Uniform1i (unifLoc, _textureCount);
					++ _textureCount;
				}
				break;
			case PipelineAttribute::ATTR_TEXTURE_VIEW_DEPTH : {
					GL::ActiveTexture (GL_TEXTURE0 + _textureCount);
					GL::BindTexture (GL_TEXTURE_2D, (unsigned int) attr [i].value.x);
					GL::TexParameteri (GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT);
					GL::Uniform1i (unifLoc, _textureCount);
					++ _textureCount;
				}
				break;
			case PipelineAttribute::ATTR_MATRIX_4X4F :
					GL::UniformMatrix4fv (unifLoc, 1, GL_FALSE, glm::value_ptr (attr [i].matrix));
				break;
			case PipelineAttribute::ATTR_BLOCK : {
					unsigned int uniBlockIndex = currentShaderView->GetUniformBlockIndex (attr [i].name.c_str ());

					if (uniBlockIndex == GL_INVALID_INDEX) {
						break;
					}

					GL::UniformBlockBinding (currentShaderView->GetProgram (), uniBlockIndex, 0);
					GL::BindBufferBase (GL_UNIFORM_BUFFER, 0, (unsigned int) attr [i].value.x);
				}
				break;
			case PipelineAttribute::ATTR_SSBO_BLOCK : {
					unsigned int ssboBlockIndex = currentShaderView->GetShaderStorageBlockIndex (attr [i].name.c_str ());

					if (ssboBlockIndex == GL_INVALID_INDEX) {
						break;
					}

					GL::ShaderStorageBlockBinding (currentShaderView->GetProgram (), ssboBlockIndex, 0);
					GL::BindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, (unsigned int) attr [i].value.x);
				}
				break;
		}
	}
}

void Pipeline::SendMaterial(Resource<MaterialView> mat, const Resource<ShaderView>& shaderView)
{
	if (mat == nullptr) {
		mat = _defaultMaterialView;
	}

	auto currentShaderView = shaderView;

	if (shaderView == nullptr) {
		currentShaderView = mat->shaderView;
	}

	SetShader (currentShaderView);

	if (_lockedShaderView != nullptr) {
		currentShaderView = _lockedShaderView;
	}

	_textureCount = 0;

	Pipeline::UpdateMatrices (currentShaderView);

	/*
	 * Set material blending mode
	*/

	// GL::Enable (GL_BLEND);
	// GL::BlendFunc (mat->blending.first, mat->blending.second);

	/*
	 * Send basic material attributes to shader
	*/

	GL::Uniform3fv (currentShaderView->GetUniformLocation ("MaterialDiffuse"), 1, glm::value_ptr (mat->diffuseColor));
	// GL::Uniform3fv (shader->GetUniformLocation ("MaterialAmbient"), 1, glm::value_ptr (mat->ambientColor));
	GL::Uniform3fv (currentShaderView->GetUniformLocation ("MaterialSpecular"), 1, glm::value_ptr (mat->specularColor));
	GL::Uniform3fv (currentShaderView->GetUniformLocation ("MaterialEmissive"), 1, glm::value_ptr (mat->emissiveColor));
	GL::Uniform1f (currentShaderView->GetUniformLocation ("MaterialShininess"), mat->shininess);
	GL::Uniform1f (currentShaderView->GetUniformLocation ("MaterialTransparency"), mat->transparency);
	GL::Uniform1f (currentShaderView->GetUniformLocation ("MaterialRefractiveIndex"), mat->refractiveIndex);

	/*
	 * Send maps to shader
	*/

	_defaultTextureView->Activate (0);
	++ _textureCount;

	// if (mat->ambientTexture) {
	// 	glActiveTexture (GL_TEXTURE0+_textureCount);
	// 	GL::BindTexture (GL_TEXTURE_2D, mat->ambientTexture);
	// 	glUniform1i (shader->GetUniformLocation ("AmbientMap"), _textureCount);
	// 	_textureCount ++;
	// } else {
	// 	glUniform1i (shader->GetUniformLocation ("AmbientMap"), 0);
	// }

	if (mat->diffuseTexture != nullptr) {
		mat->diffuseTexture->Activate (_textureCount);
		GL::Uniform1i (currentShaderView->GetUniformLocation ("DiffuseMap"), _textureCount);
		++ _textureCount;
	} else {
		GL::Uniform1i (currentShaderView->GetUniformLocation ("DiffuseMap"), 0);
	}

	if (mat->specularTexture != nullptr) {
		mat->specularTexture->Activate (_textureCount);
		GL::Uniform1i (currentShaderView->GetUniformLocation ("SpecularMap"), _textureCount);
		++ _textureCount;
	} else {
		GL::Uniform1i (currentShaderView->GetUniformLocation ("SpecularMap"), 0);
	}

	if (mat->emissiveTexture != nullptr) {
		mat->emissiveTexture->Activate (_textureCount);
		GL::Uniform1i (currentShaderView->GetUniformLocation ("EmissiveMap"), _textureCount);

		++ _textureCount;
	} else {
		GL::Uniform1i (currentShaderView->GetUniformLocation ("EmissiveMap"), 0);
	}

	if (mat->bumpTexture != nullptr) {
		mat->bumpTexture->Activate (_textureCount);
		GL::Uniform1i (currentShaderView->GetUniformLocation ("NormalMap"), _textureCount);
		++ _textureCount;
	} else {
		GL::Uniform1i (currentShaderView->GetUniformLocation ("NormalMap"), 0);
	}

	if (mat->alphaTexture != nullptr) {
		mat->alphaTexture->Activate (_textureCount);
		GL::Uniform1i (currentShaderView->GetUniformLocation ("AlphaMap"), _textureCount);
		++ _textureCount;
	} else {
		GL::Uniform1i (currentShaderView->GetUniformLocation ("AlphaMap"), 0);
	}

	/*
	 * Send custom attributes
	*/

	// for (std::size_t k = 0;k<mat->attributes.size ();k++) 
	// {
	// 	if (mat->attributes [k].type == Attribute::AttrType::ATTR_TEXTURE2D
	// 		|| mat->attributes [k].type == Attribute::AttrType::ATTR_TEXTURE2D_ATLAS) {
	// 		GL::ActiveTexture (GL_TEXTURE0 + _textureCount);
	// 		// Texture* tex = TextureManager::Instance ()->GetTexture (mat->attributes [k].valueName);
	// 		// unsigned int textureID = tex->GetGPUIndex ();
	// 		// GL::BindTexture (GL_TEXTURE_2D, textureID);
	// 		// GL::Uniform1i (shader->GetUniformLocation (mat->attributes [k].name.c_str ()), _textureCount);

	// 		++ _textureCount;
	// 	}
	// 	else if (mat->attributes [k].type == Attribute::AttrType::ATTR_TEXTURE_CUBE) {
	// 		GL::ActiveTexture (GL_TEXTURE0 + _textureCount);

	// 		unsigned int textureID = 0;
	// 		if (mat->attributes [k].values.x == 1) {
	// 			textureID = (unsigned int)mat->attributes [k].values.y;
	// 		}

	// 		GL::BindTexture (GL_TEXTURE_CUBE_MAP, textureID);
	// 		GL::Uniform1i (shader->GetUniformLocation (mat->attributes [k].name.c_str ()), _textureCount);

	// 		++ _textureCount;
	// 	}
	// 	else if (mat->attributes [k].type == Attribute::AttrType::ATTR_FLOAT) {
	// 		float value = mat->attributes [k].values.x;
	// 		GL::Uniform1f (shader->GetUniformLocation (mat->attributes [k].name.c_str ()), value);
	// 	}
	// 	else if (mat->attributes [k].type == Attribute::AttrType::ATTR_VEC3) {
	// 		glm::vec3 values = mat->attributes [k].values;
	// 		GL::Uniform3f (shader->GetUniformLocation (mat->attributes [k].name.c_str ()), 
	// 			values.x, values.y, values.z);
	// 	}
	// }
}
