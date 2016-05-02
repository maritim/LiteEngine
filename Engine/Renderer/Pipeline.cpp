#include "Pipeline.h"

#include <GL/glew.h>

#include "Lighting/LightsManager.h"
#include "Lighting/Light.h"
#include "Texture/Texture.h"
#include "SceneGraph/Transform.h"
#include "Material/Material.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"
#include "Skybox/Skybox.h"

#include "PipelineAttribute.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Math/glm/glm.hpp"
#include "Core/Math/glm/gtc/matrix_transform.hpp"
#include "Core/Math/glm/gtc/type_ptr.hpp"

glm::mat4 Pipeline::_modelMatrix (0);
glm::mat4 Pipeline::_viewMatrix (0);
glm::mat4 Pipeline::_projectionMatrix (0);
glm::vec3 Pipeline::_cameraPosition;
std::size_t Pipeline::_textureCount (0);

void Pipeline::SetShader (Shader* shader)
{
	GL::UseProgram (shader->GetProgram ());

	_textureCount = 0;
}

void Pipeline::CreatePerspective (float FOV, float aspect, float zNear, float zFar)
{
	_projectionMatrix = glm::perspective (FOV, aspect, zNear, zFar);
}

void Pipeline::SendCamera (Camera* camera)
{
	Vector3 position = camera->GetPosition ();
	_cameraPosition = glm::vec3 (position.x, position.y, position.z);

	_viewMatrix = glm::mat4 (1.f);

	Vector3 eulerAngle = camera->ToVector3 ();
	glm::vec3 euler (eulerAngle.x, eulerAngle.y, eulerAngle.z);
	euler = glm::normalize (euler);

	glm::vec3 zaxis = glm::normalize (euler * glm::vec3 (-1, -1, -1));
	glm::vec3 xaxis = glm::normalize (glm::cross (glm::vec3 (0, 1, 0), zaxis));
	glm::vec3 yaxis = glm::cross (zaxis, xaxis);

	_viewMatrix [0][0] = xaxis.x;
	_viewMatrix [0][1] = yaxis.x;
	_viewMatrix [0][2] = zaxis.x;

	_viewMatrix [1][0] = xaxis.y;
	_viewMatrix [1][1] = yaxis.y;
	_viewMatrix [1][2] = zaxis.y;

	_viewMatrix [2][0] = xaxis.z;
	_viewMatrix [2][1] = yaxis.z;
	_viewMatrix [2][2] = zaxis.z;

	_viewMatrix =  glm::translate (_viewMatrix, glm::vec3 (-position.x, -position.y, -position.z));		
}

void Pipeline::SetObjectTransform (Transform* transform)
{
	Vector3 position = transform->GetPosition ();
	Vector3 scalev = transform->GetScale ();
	Vector3 rotationv = transform->GetRotation ();

	glm::mat4 translate = glm::translate (glm::mat4 (1.f), glm::vec3 (position.x, position.y, position.z));
	glm::mat4 scale = glm::scale (glm::mat4 (1.f), glm::vec3 (scalev.x, scalev.y, scalev.z));
	glm::mat4 rotation =  glm::rotate (glm::mat4 (1.0f), rotationv.y, glm::vec3 (0.0f, 1.0f, 0.0f)) * 
		glm::rotate (glm::mat4 (1.0f), rotationv.z, glm::vec3 (0.0f, 0.0f, 1.0f)) *
		glm::rotate (glm::mat4 (1.0f), rotationv.x, glm::vec3 (1.0f, 0.0f, 0.0f));

	_modelMatrix = translate * scale * rotation;
}

void Pipeline::UpdateMatrices (Shader* shader)
{
	glm::mat4 modelViewMatrix = _viewMatrix * _modelMatrix;
	glm::mat4 modelViewProjectionMatrix = _projectionMatrix * _viewMatrix * _modelMatrix;

	glm::mat3 normalWorldMatrix = glm::transpose (glm::inverse (glm::mat3 (modelViewMatrix)));
	glm::mat3 normalMatrix = glm::transpose (glm::inverse (glm::mat3 (_modelMatrix)));

	glUniformMatrix4fv (shader->GetUniformLocation ("modelMatrix"), 1, GL_FALSE, glm::value_ptr (_modelMatrix));
	glUniformMatrix4fv (shader->GetUniformLocation ("viewMatrix"), 1, GL_FALSE, glm::value_ptr (_viewMatrix));
	glUniformMatrix4fv (shader->GetUniformLocation ("modelViewMatrix"), 1, GL_FALSE, glm::value_ptr (modelViewMatrix));
	glUniformMatrix4fv (shader->GetUniformLocation ("projectionMatrix"), 1, GL_FALSE, glm::value_ptr (_projectionMatrix));
	glUniformMatrix4fv (shader->GetUniformLocation ("modelViewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr (modelViewProjectionMatrix));
	glUniformMatrix3fv (shader->GetUniformLocation ("normalMatrix"), 1, GL_FALSE, glm::value_ptr (normalMatrix));
	glUniformMatrix3fv (shader->GetUniformLocation ("normalWorldMatrix"), 1, GL_FALSE, glm::value_ptr (normalWorldMatrix));
	glUniform3fv (shader->GetUniformLocation ("cameraPosition"), 1, glm::value_ptr (_cameraPosition));

	SendLights (shader);
}

void Pipeline::SendLights (Shader* shader)
{
	Vector3 vec = LightsManager::Instance ()->GetAmbientColorLight ();
	Color color;

	glUniform3f(shader->GetUniformLocation ("sceneAmbient"), vec.x, vec.y, vec.z);

	const int lightsLimit = 3;

	int spotLightCount = 0;
	int pointLightCount = 0;
	int directionalLightCount = 0;

	for (std::size_t i=0;i<LightsManager::Instance ()->Size ();i++) {
		Light* light = LightsManager::Instance ()->GetLight (i);
		std::string address;

		glm::vec4 vec4 (vec.x, vec.y, vec.z, 1.0);

		switch (light->GetType ()) {
			case Light::Type::DIRECTIONAL_LIGHT :
				if (directionalLightCount >= lightsLimit) {
					break;
				}

				vec = light->GetTransform ()->GetPosition ();
				//vec4 = glm::vec4 (vec.x, vec.y, vec.z, 0.0);
				address = "directionalLights[" + std::to_string (directionalLightCount) + "].position";
				glUniform4f (glGetUniformLocation (shader->GetProgram (), address.c_str ()), vec.x, vec.y, vec.z, 0.0);

				vec = light->GetColor ().ToVector3 ();
				address = "directionalLights[" + std::to_string (directionalLightCount) + "].diffuse";
				glUniform4f (shader->GetUniformLocation (address.c_str ()), vec.x, vec.y, vec.z, 1.0);

				vec = light->GetSpecularColor ().ToVector3 ();
				address = "directionalLights[" + std::to_string (directionalLightCount) + "].specular";
				glUniform4f (shader->GetUniformLocation (address.c_str ()), vec.x, vec.y, vec.z, 1.0);

				++ directionalLightCount;

				break;
			case Light::Type::POINT_LIGHT :
				if (pointLightCount >= lightsLimit) {
					break;
				}

				vec = light->GetTransform ()->GetPosition ();
				vec4 = glm::vec4 (vec.x, vec.y, vec.z, 1.0);
				address = "pointLights[" + std::to_string (pointLightCount) + "].position";
				glUniform4fv (shader->GetUniformLocation (address.c_str ()), 1, glm::value_ptr (vec4));

				vec = light->GetColor ().ToVector3 ();
				address = "pointLights[" + std::to_string (pointLightCount) + "].diffuse";
				glUniform4f (shader->GetUniformLocation (address.c_str ()), vec.x, vec.y, vec.z, 1.0);

				vec = light->GetColor ().ToVector3 ();
				address = "pointLights[" + std::to_string (pointLightCount) + "].specular";
				glUniform4f (shader->GetUniformLocation (address.c_str ()), vec.x, vec.y, vec.z, 1.0);

				address = "pointLights[" + std::to_string (pointLightCount) + "].constantAttenuation";
				glUniform1f (shader->GetUniformLocation (address.c_str ()), light->GetConstantAttenuation ());

				address = "pointLights[" + std::to_string (pointLightCount) + "].linearAttenuation";
				glUniform1f (shader->GetUniformLocation (address.c_str ()), light->GetLinearAttenuation ());

				address = "pointLights[" + std::to_string (pointLightCount) + "].quadraticAttenuation";
				glUniform1f (shader->GetUniformLocation (address.c_str ()), light->GetQuadraticAttenuation ());

				++ pointLightCount;

				break;
			case Light::Type::SPOT_LIGHT :
				if (spotLightCount >= lightsLimit) {
					break;
				}

				vec = light->GetTransform ()->GetPosition ();
				address = "spotLights[" + std::to_string (spotLightCount) + "].position";
				glUniform4f (shader->GetUniformLocation (address.c_str ()), vec.x, vec.y, vec.z, 1.0);

				vec = light->GetColor ().ToVector3 ();
				address = "spotLights[" + std::to_string (spotLightCount) + "].diffuse";
				glUniform4f (shader->GetUniformLocation (address.c_str ()), vec.x, vec.y, vec.z, 1.0);

				vec = light->GetColor ().ToVector3 ();
				address = "spotLights[" + std::to_string (spotLightCount) + "].specular";
				glUniform4f (shader->GetUniformLocation (address.c_str ()), vec.x, vec.y, vec.z, 1.0);

				address = "spotLights[" + std::to_string (spotLightCount) + "].constantAttenuation";
				glUniform1f (shader->GetUniformLocation (address.c_str ()), light->GetConstantAttenuation ());

				address = "spotLights[" + std::to_string (spotLightCount) + "].linearAttenuation";
				glUniform1f (shader->GetUniformLocation (address.c_str ()), light->GetLinearAttenuation ());

				address = "spotLights[" + std::to_string (spotLightCount) + "].quadraticAttenuation";
				glUniform1f (shader->GetUniformLocation (address.c_str ()), light->GetQuadraticAttenuation ());

				address = "spotLights[" + std::to_string (spotLightCount) + "].spotCutoff";
				glUniform1f (shader->GetUniformLocation (address.c_str ()), light->GetSpotCutoff ());

				address = "spotLights[" + std::to_string (spotLightCount) + "].spotExponent";
				glUniform1f (shader->GetUniformLocation (address.c_str ()), light->GetSpotExponent ());

				vec = light->GetSpotDirection ();
				address = "spotLights[" + std::to_string (spotLightCount) + "].spotDirection";
				glUniform3f (shader->GetUniformLocation (address.c_str ()), vec.x, vec.y, vec.z);

				++ spotLightCount;

				break;
		}
	}

	glUniform1i (shader->GetUniformLocation ("spotLightCount"), spotLightCount);
	glUniform1i (shader->GetUniformLocation ("pointLightCount"), pointLightCount);
	glUniform1i (shader->GetUniformLocation ("directionalLightCount"), directionalLightCount);
}

void Pipeline::SendCustomAttributes (const std::string& shaderName, const std::vector<PipelineAttribute>& attr)
{
	Shader* shader = ShaderManager::Instance ()->GetShader (shaderName);

	for (std::size_t i=0;i<attr.size ();i++) {

		int unifLoc = shader->GetUniformLocation (attr [i].name.c_str ());

		switch (attr [i].type) {
			case PipelineAttribute::ATTR_1I :
				glUniform1i (unifLoc, (int) attr [i].value.x);
				break;
			case PipelineAttribute::ATTR_2I :
				glUniform2i (unifLoc, (int) attr [i].value.x,
					(int) attr [i].value.y);
				break;
			case PipelineAttribute::ATTR_3I :
				glUniform3i (unifLoc, (int) attr [i].value.x,
					(int) attr [i].value.y, (int) attr [i].value.z);
				break;
			case PipelineAttribute::ATTR_1F :
				glUniform1f (unifLoc, attr [i].value.x);
				break;
			case PipelineAttribute::ATTR_2F :
				glUniform2f (unifLoc, attr [i].value.x,
					attr [i].value.y);
				break;
			case PipelineAttribute::ATTR_3F :
				glUniform3f (unifLoc, attr [i].value.x,
					attr [i].value.y, attr [i].value.z);
				break;
			case PipelineAttribute::ATTR_TEXTURE_2D :	{
					glActiveTexture (GL_TEXTURE0 + _textureCount);
					glBindTexture (GL_TEXTURE_2D, (unsigned int) attr [i].value.x);
					glUniform1i (unifLoc, _textureCount);
					++ _textureCount;
				}
				break;
			case PipelineAttribute::ATTR_TEXTURE_CUBE : {
					glActiveTexture (GL_TEXTURE0 + _textureCount);
					glBindTexture (GL_TEXTURE_CUBE_MAP, (unsigned int) attr [i].value.x);
					glUniform1i (unifLoc, _textureCount);
					++ _textureCount;
				}
				break;
		}
	}
}

void Pipeline::SendMaterial(Material* mat)
{
	Shader* shader = ShaderManager::Instance ()->GetShader (mat->shaderName);

	if (shader == nullptr) {
		shader = ShaderManager::Instance ()->GetShader ("DEFAULT");
	}

	SetShader (shader);

	Pipeline::UpdateMatrices (shader);

	// Send basic material attributes to shader
	glUniform4f (shader->GetUniformLocation ("MaterialDiffuse"), mat->diffuseColor.x, 
		mat->diffuseColor.y, mat->diffuseColor.z, 1.0);
	glUniform4f (shader->GetUniformLocation ("MaterialAmbient"), mat->ambientColor.x,
		mat->ambientColor.y, mat->ambientColor.z, 1.0);
	glUniform4f (shader->GetUniformLocation ("MaterialSpecular"), mat->specularColor.x,
		mat->specularColor.y, mat->specularColor.z, 1.0);
	glUniform1f (shader->GetUniformLocation ("MaterialShininess"), mat->shininess);
	glUniform1f (shader->GetUniformLocation ("MaterialTransparency"), mat->transparency);

	// Send maps to shader
	glActiveTexture (GL_TEXTURE0);
	GL::BindTexture (GL_TEXTURE_2D, TextureManager::Instance ().Default ()->GetGPUIndex ());
	++ _textureCount;

	if (mat->ambientTexture) {
		glActiveTexture (GL_TEXTURE0+_textureCount);
		GL::BindTexture (GL_TEXTURE_2D, mat->ambientTexture);
		glUniform1i (shader->GetUniformLocation ("AmbientMap"), _textureCount);
		_textureCount ++;
	} else {
		glUniform1i (shader->GetUniformLocation ("AmbientMap"), 0);
	}

	if (mat->diffuseTexture) {
		glActiveTexture (GL_TEXTURE0+_textureCount);
		GL::BindTexture (GL_TEXTURE_2D, mat->diffuseTexture);
		glUniform1i (shader->GetUniformLocation ("DiffuseMap"), _textureCount);
		++ _textureCount;
	} else {
		glUniform1i (shader->GetUniformLocation ("DiffuseMap"), 0);
	}

	if (mat->specularTexture) {
		glActiveTexture (GL_TEXTURE0 + _textureCount);
		GL::BindTexture (GL_TEXTURE_2D, mat->specularTexture);
		glUniform1i (shader->GetUniformLocation ("SpecularMap"), _textureCount);
		++ _textureCount;
	} else {
		glUniform1i (shader->GetUniformLocation ("SpecularMap"), 0);
	}

	if (mat->alphaTexture) {
		glActiveTexture (GL_TEXTURE0 + _textureCount);
		GL::BindTexture (GL_TEXTURE_2D, mat->alphaTexture);
		glUniform1i (shader->GetUniformLocation ("AlphaMap"), _textureCount);
		++ _textureCount;
	} else {
		glUniform1i (shader->GetUniformLocation ("AlphaMap"), 0);
	}

	// Send custom attributes

	for (std::size_t k = 0;k<mat->attributes.size ();k++) 
	{
		if (mat->attributes [k].type == Attribute::AttrType::ATTR_TEXTURE2D
			|| mat->attributes [k].type == Attribute::AttrType::ATTR_TEXTURE2D_ATLAS) {
			glActiveTexture (GL_TEXTURE0 + _textureCount);
			Texture* tex = TextureManager::Instance ().GetTexture (mat->attributes [k].valueName);
			unsigned int textureID = tex->GetGPUIndex ();
			glBindTexture (GL_TEXTURE_2D, textureID);
			glUniform1i (shader->GetUniformLocation (mat->attributes [k].name.c_str ()), _textureCount);

			++ _textureCount;
		}
		else if (mat->attributes [k].type == Attribute::AttrType::ATTR_TEXTURE_CUBE) {
			glActiveTexture (GL_TEXTURE0 + _textureCount);

			unsigned int textureID = 0;
			if (mat->attributes [k].values.x == 0) {
				textureID = Skybox::GetCurrentCubeMap ()->GetGPUIndex ();
			}
			else if (mat->attributes [k].values.x == 1) {
				textureID = (unsigned int)mat->attributes [k].values.y;
			}

			glBindTexture (GL_TEXTURE_CUBE_MAP, textureID);
			glUniform1i (shader->GetUniformLocation (mat->attributes [k].name.c_str ()), _textureCount);

			++ _textureCount;
		}
		else if (mat->attributes [k].type == Attribute::AttrType::ATTR_FLOAT) {
			float value = mat->attributes [k].values.x;
			glUniform1f (shader->GetUniformLocation (mat->attributes [k].name.c_str ()), value);
		}
		else if (mat->attributes [k].type == Attribute::AttrType::ATTR_VEC3) {
			Vector3 values = mat->attributes [k].values;
			glUniform3f (shader->GetUniformLocation (mat->attributes [k].name.c_str ()), 
				values.x, values.y, values.z);
		}
	}
}