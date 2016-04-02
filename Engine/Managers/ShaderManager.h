#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <string>
#include <map>

#include "Shader/Shader.h"

class ShaderManager
{
private:
	static std::map<std::string, Shader*> _shaderCollection;

public:
	static int AddShader (const std::string& shaderName,
		const std::string& vertexFile, const std::string& fragmentFile);
	static int DeleteShader (const std::string& shaderName);
	static Shader* GetShader (const std::string& shaderName);
	static void Clear ();

private:
	static void LoadShaderFile (const std::string& fileName, std::string& content);
	static unsigned int LoadShader (const std::string& content, unsigned int mode);
	static void ErrorCheck (unsigned int shader);
};

#endif