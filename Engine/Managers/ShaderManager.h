#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <string>
#include <map>

class ShaderManager
{
private:
	static std::map<std::string, unsigned int> _shaderCollection;
	static std::map<unsigned int, std::pair<unsigned int, unsigned int>> _programShadersLink;

public:
	static int AddShader (const std::string& shaderName,
		const std::string& vertexFile, const std::string& fragmentFile);
	static int DeleteShader (const std::string& shaderName);
	static unsigned int GetShader (const std::string& shaderName);
	static void Clear ();

private:
	static void LoadShaderFile (const std::string& fileName, std::string& content);
	static unsigned int LoadShader (const std::string& content, unsigned int mode);
};

#endif