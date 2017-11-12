#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <string>
#include <map>

#include "Shader/Shader.h"
#include "Shader/DrawingShader.h"
#include "Shader/ComputeShader.h"

class ShaderManager : public Singleton<ShaderManager>
{
	friend Singleton<ShaderManager>;

private:
	std::map<std::string, Shader*> _shaderCollection;

public:
	GLuint AddShader (const std::string& shaderName,
		const std::string& vertexFile, const std::string& fragmentFile, 
		const std::string& geometryFile = "");
	GLuint AddComputeShader (const std::string& shaderName,
		const std::string& computeFile);
	int DeleteShader (const std::string& shaderName);

	Shader* GetShader (const std::string& shaderName);

	void Clear();

private:
	ShaderManager ();
	~ShaderManager ();
	ShaderManager (const ShaderManager&);
	ShaderManager& operator=(const ShaderManager&);

	unsigned int BuildShaderFile (unsigned int program, int shaderType,
		const std::string& fileName);
	void LoadShaderFile (const std::string& fileName, std::string& content);
	unsigned int LoadShader (const std::string& content, unsigned int mode);
	void ErrorCheck (unsigned int shader);
};

#endif