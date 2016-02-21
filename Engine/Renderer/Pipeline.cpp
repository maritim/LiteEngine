#include "Pipeline.h"

#include <GL/glew.h>
#include <cmath>

#include "Lighting/Lights.h"
#include "Texture/Texture.h"
#include "SceneGraph/Transform.h"
#include "Material/Material.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"
#include "Skybox/Skybox.h"

#include "PipelineAttribute.h"

Matrix Pipeline::_modelMatrix (4, 4);
Matrix Pipeline::_viewMatrix (4, 4);
Matrix Pipeline::_projectionMatrix (4, 4);
Vector3 Pipeline::_cameraPosition;
std::size_t Pipeline::_textureCount (0);

inline float ctan (float x)
{
	return 1.0f / tan (x);
}

void Pipeline::SetShader (unsigned int shader)
{
	glUseProgram (shader);

	_textureCount = 0;
}

void Pipeline::CreatePerspective (float FOV, float aspect, float zNear, float zFar)
{
	_projectionMatrix = Matrix::Zero (4, 4);

	float f = ctan (FOV / 2);

	_projectionMatrix [0][0] = f / aspect;
	_projectionMatrix [1][1] = f;
	_projectionMatrix [2][2] = (zNear + zFar) / (zNear - zFar);
	_projectionMatrix [3][2] = -1;
	_projectionMatrix [2][3] = (2 * zNear * zFar) / (zNear - zFar);

	// _projectionMatrix [0][0] = f / aspect;
	// _projectionMatrix [1][1] = f;
	// _projectionMatrix [2][2] = (zNear + zFar) / (zNear - zFar);
	// _projectionMatrix [3][2] = -1;
	// _projectionMatrix [2][3] = (2 * zNear * zFar) / (zNear - zFar);	
}

void Pipeline::SetCameraPosition (Vector3 position)
{
	_cameraPosition = position;

	_viewMatrix = _viewMatrix * Matrix::Translate (-position.x, -position.y, -position.z);
}

void Pipeline::SetCameraRotation (Vector3 eulerAngle)
{
	_viewMatrix = Matrix::Identity (4);

	eulerAngle.Normalize ();

    Vector3 zaxis = (eulerAngle * -1).Normalized ();    
    Vector3 xaxis = (Vector3::Up.Cross (zaxis)).Normalized ();
    Vector3 yaxis = zaxis.Cross (xaxis);     

    _viewMatrix [0][0] = xaxis.x;
    _viewMatrix [1][0] = yaxis.x;
    _viewMatrix [2][0] = zaxis.x;

    _viewMatrix [0][1] = xaxis.y;
    _viewMatrix [1][1] = yaxis.y;
    _viewMatrix [2][1] = zaxis.y;

    _viewMatrix [0][2] = xaxis.z;
    _viewMatrix [1][2] = yaxis.z;
    _viewMatrix [2][2] = zaxis.z;
}

void Pipeline::SetObjectTransform (Transform* transform)
{
	Vector3 position = transform->GetPosition ();
	Vector3 scalev = transform->GetScale ();
	Vector3 rotationv = transform->GetRotation ();

	Matrix translate = Matrix::Translate (position.x, position.y, position.z);
	Matrix scale = Matrix::Scale (scalev.x, scalev.y, scalev.z);
	Matrix rotation = Matrix::Rotate (rotationv.x, rotationv.y, rotationv.z);

	_modelMatrix = translate * scale * rotation;
}

void Pipeline::UpdateMatrices (unsigned int program)
{
	Matrix modelViewMatrix = _viewMatrix * _modelMatrix;
	Matrix modelViewProjectionMatrix = _projectionMatrix * _viewMatrix * _modelMatrix;

	Matrix normalMatrix (3, 3);

	for (int i=0;i<3;i++) {
		for (int j=0;j<3;j++) {
			normalMatrix [i][j] = modelViewMatrix [i][j];
		}
	}

	normalMatrix = normalMatrix.Inverse ().Transpose ();

	Matrix normalWorldMatrix (3, 3);

	for (int i=0;i<3;i++) {
		for (int j=0;j<3;j++) {
			normalWorldMatrix [i][j] = _modelMatrix [i][j];
		}
	}

	normalWorldMatrix = normalWorldMatrix.Inverse ().Transpose ();

	float* matrix = _modelMatrix.C_Matrix ();
	glUniformMatrix4fv (glGetUniformLocation (program, "modelMatrix"), 1, GL_TRUE, matrix);
	delete matrix;

	matrix = _viewMatrix.C_Matrix ();
	glUniformMatrix4fv (glGetUniformLocation (program, "viewMatrix"), 1, GL_TRUE, matrix);
	delete matrix;

	matrix = modelViewMatrix.C_Matrix ();
	glUniformMatrix4fv (glGetUniformLocation (program, "modelViewMatrix"), 1, GL_TRUE, matrix);
	delete matrix;

	matrix = _projectionMatrix.C_Matrix ();
	glUniformMatrix4fv (glGetUniformLocation (program, "projectionMatrix"), 1, GL_TRUE, matrix);
	delete matrix;

	matrix = modelViewProjectionMatrix.C_Matrix ();
	glUniformMatrix4fv (glGetUniformLocation (program, "modelViewProjectionMatrix"), 1, GL_TRUE, matrix);
	delete matrix;

	matrix = normalMatrix.C_Matrix ();
	glUniformMatrix3fv (glGetUniformLocation (program, "normalMatrix"), 1, GL_TRUE, matrix);
	delete matrix;

	matrix = normalWorldMatrix.C_Matrix ();
	glUniformMatrix3fv (glGetUniformLocation (program, "normalWorldMatrix"), 1, GL_TRUE, matrix);
	delete matrix;

	glUniform3f(glGetUniformLocation(program,"cameraPosition"),_cameraPosition.x,
		_cameraPosition.y, _cameraPosition.z);

	SendLights (program);
}

void Pipeline::SendLights (unsigned int program)
{
	Vector3 vec = Lights::Instance ().GetAmbientColorLight ();

	glUniform3f(glGetUniformLocation(program,"sceneAmbient"), vec.x, vec.y, vec.z);

	const int lightsLimit = 3;

	int spotLightCount = 0;
	int pointLightCount = 0;
	int directionalLightCount = 0;

	for (std::size_t i=0;i<Lights::Instance ().Size ();i++) {
		Light* light = Lights::Instance ().GetLight (i);
		std::string address;

		switch (light->type) {
			case Light::Type::DIRECTIONAL_LIGHT :
				if (directionalLightCount >= lightsLimit) {
					break;
				}

				vec = light->position;
				address = "directionalLights[" + std::to_string (directionalLightCount) + "].position";
				glUniform4f (glGetUniformLocation (program, address.c_str ()), vec.x, vec.y, vec.z, 0.0);

				vec = light->color;
				address = "directionalLights[" + std::to_string (directionalLightCount) + "].diffuse";
				glUniform4f (glGetUniformLocation (program, address.c_str ()), vec.x, vec.y, vec.z, 1.0);

				vec = light->specularColor;
				address = "directionalLights[" + std::to_string (directionalLightCount) + "].specular";
				glUniform4f (glGetUniformLocation (program, address.c_str ()), vec.x, vec.y, vec.z, 1.0);

				++ directionalLightCount;

				break;
			case Light::Type::POINT_LIGHT :
				if (pointLightCount >= lightsLimit) {
					break;
				}

				vec = light->position;
				address = "pointLights[" + std::to_string (pointLightCount) + "].position";
				glUniform4f (glGetUniformLocation (program, address.c_str ()), vec.x, vec.y, vec.z, 1.0);

				vec = light->color;
				address = "pointLights[" + std::to_string (pointLightCount) + "].diffuse";
				glUniform4f (glGetUniformLocation (program, address.c_str ()), vec.x, vec.y, vec.z, 1.0);

				vec = light->specularColor;
				address = "pointLights[" + std::to_string (pointLightCount) + "].specular";
				glUniform4f (glGetUniformLocation (program, address.c_str ()), vec.x, vec.y, vec.z, 1.0);

				address = "pointLights[" + std::to_string (pointLightCount) + "].constantAttenuation";
				glUniform1f (glGetUniformLocation (program, address.c_str ()), light->constantAttenuation);

				address = "pointLights[" + std::to_string (pointLightCount) + "].linearAttenuation";
				glUniform1f (glGetUniformLocation (program, address.c_str ()), light->linearAttenuation);

				address = "pointLights[" + std::to_string (pointLightCount) + "].quadraticAttenuation";
				glUniform1f (glGetUniformLocation (program, address.c_str ()), light->quadraticAttenuation);

				++ pointLightCount;

				break;
			case Light::Type::SPOT_LIGHT :
				if (spotLightCount >= lightsLimit) {
					break;
				}

				vec = light->position;
				address = "spotLights[" + std::to_string (spotLightCount) + "].position";
				glUniform4f (glGetUniformLocation (program, address.c_str ()), vec.x, vec.y, vec.z, 1.0);

				vec = light->color;
				address = "spotLights[" + std::to_string (spotLightCount) + "].diffuse";
				glUniform4f (glGetUniformLocation (program, address.c_str ()), vec.x, vec.y, vec.z, 1.0);

				vec = light->specularColor;
				address = "spotLights[" + std::to_string (spotLightCount) + "].specular";
				glUniform4f (glGetUniformLocation (program, address.c_str ()), vec.x, vec.y, vec.z, 1.0);

				address = "spotLights[" + std::to_string (spotLightCount) + "].constantAttenuation";
				glUniform1f (glGetUniformLocation (program, address.c_str ()), light->constantAttenuation);

				address = "spotLights[" + std::to_string (spotLightCount) + "].linearAttenuation";
				glUniform1f (glGetUniformLocation (program, address.c_str ()), light->linearAttenuation);

				address = "spotLights[" + std::to_string (spotLightCount) + "].quadraticAttenuation";
				glUniform1f (glGetUniformLocation (program, address.c_str ()), light->quadraticAttenuation);

				address = "spotLights[" + std::to_string (spotLightCount) + "].spotCutoff";
				glUniform1f (glGetUniformLocation (program, address.c_str ()), light->spotCutoff);

				address = "spotLights[" + std::to_string (spotLightCount) + "].spotExponent";
				glUniform1f (glGetUniformLocation (program, address.c_str ()), light->spotExponent);

				vec = light->spotDirection;
				address = "spotLights[" + std::to_string (spotLightCount) + "].spotDirection";
				glUniform3f (glGetUniformLocation (program, address.c_str ()), vec.x, vec.y, vec.z);

				++ spotLightCount;

				break;
		}
	}

	glUniform1i (glGetUniformLocation(program, "spotLightCount"), spotLightCount);
	glUniform1i (glGetUniformLocation(program, "pointLightCount"), pointLightCount);
	glUniform1i (glGetUniformLocation(program, "directionalLightCount"), directionalLightCount);
}

void Pipeline::SendCustomAttributes (const std::string& shaderName, const std::vector<PipelineAttribute>& attr)
{
	unsigned int program = ShaderManager::GetShader (shaderName);

	for (std::size_t i=0;i<attr.size ();i++) {

		int unifLoc = glGetUniformLocation (program, attr [i].name.c_str ());

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
//			std::cout << mat->shaderName << "\n";

	unsigned int program = ShaderManager::GetShader (mat->shaderName);

	if (program == 0) {
		program = ShaderManager::GetShader ("DEFAULT");
	}

	SetShader (program);

	Pipeline::UpdateMatrices (program);

	// Send basic material attributes to shader
	glUniform4f (glGetUniformLocation (program, "MaterialDiffuse"), mat->diffuseColor.x, 
		mat->diffuseColor.y, mat->diffuseColor.z, 1.0);
	glUniform4f (glGetUniformLocation (program, "MaterialAmbient"), mat->ambientColor.x,
		mat->ambientColor.y, mat->ambientColor.z, 1.0);
	glUniform4f (glGetUniformLocation (program, "MaterialSpecular"), mat->specularColor.x,
		mat->specularColor.y, mat->specularColor.z, 1.0);
	glUniform1f (glGetUniformLocation (program, "MaterialShininess"), mat->shininess);
	glUniform1f (glGetUniformLocation (program, "MaterialTransparency"), mat->transparency);

	// Send maps to shader
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, TextureManager::Instance ().Default ()->GetGPUIndex ());
	++ _textureCount;

	if (mat->ambientTexture) {
		glActiveTexture (GL_TEXTURE0+_textureCount);
		glBindTexture (GL_TEXTURE_2D, mat->ambientTexture);
		glUniform1i (glGetUniformLocation (program, "AmbientMap"), _textureCount);
		_textureCount ++;
	} else {
		glUniform1i (glGetUniformLocation (program, "AmbientMap"), 0);
	}

	if (mat->diffuseTexture) {
		glActiveTexture (GL_TEXTURE0+_textureCount);
		glBindTexture (GL_TEXTURE_2D, mat->diffuseTexture);
		glUniform1i (glGetUniformLocation (program, "DiffuseMap"), _textureCount);
		++ _textureCount;
	} else {
		glUniform1i (glGetUniformLocation (program, "DiffuseMap"), 0);
	}

	if (mat->specularTexture) {
		glActiveTexture (GL_TEXTURE0 + _textureCount);
		glBindTexture (GL_TEXTURE_2D, mat->specularTexture);
		glUniform1i (glGetUniformLocation (program, "SpecularMap"), _textureCount);
		++ _textureCount;
	} else {
		glUniform1i (glGetUniformLocation (program, "SpecularMap"), 0);
	}

	if (mat->alphaTexture) {
		glActiveTexture (GL_TEXTURE0 + _textureCount);
		glBindTexture (GL_TEXTURE_2D, mat->alphaTexture);
		glUniform1i (glGetUniformLocation (program, "AlphaMap"), _textureCount);
		++ _textureCount;
	} else {
		glUniform1i (glGetUniformLocation (program, "AlphaMap"), 0);
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
			glUniform1i (glGetUniformLocation (program, mat->attributes [k].name.c_str ()), _textureCount);

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
			glUniform1i (glGetUniformLocation (program, mat->attributes [k].name.c_str ()), _textureCount);

			++ _textureCount;
		}
		else if (mat->attributes [k].type == Attribute::AttrType::ATTR_FLOAT) {
			float value = mat->attributes [k].values.x;
			glUniform1f (glGetUniformLocation (program, mat->attributes [k].name.c_str ()), value);
		}
		else if (mat->attributes [k].type == Attribute::AttrType::ATTR_VEC3) {
			Vector3 values = mat->attributes [k].values;
			glUniform3f (glGetUniformLocation (program, mat->attributes [k].name.c_str ()), 
				values.x, values.y, values.z);
		}
	}
}