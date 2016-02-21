#include "ShaderManager.h"

#include <GL/glew.h>
#include <string>
#include <map>
#include <fstream>
#include <vector>

#include "Core/Console/Console.h"

std::map<std::string, unsigned int> ShaderManager::_shaderCollection;
std::map<unsigned int, std::pair<unsigned int, unsigned int>> ShaderManager::_programShadersLink;

// medium priority TODO: Improve this
void ShaderManager::LoadShaderFile (const std::string& filename, std::string& content)
{
	std::ifstream f(filename);

	if (!f.is_open ()) {
		Console::LogError ("File " + filename + " could not be opened!");
		return ;
	}

	char line[300];

	while (!f.eof ()) {
		f.getline (line, 300);
		content += line;
		content += "\n";
	}

	f.close ();
}

unsigned int ShaderManager::LoadShader (const std::string& source, unsigned int mode)
{
	unsigned int id;
	id = glCreateShader (mode);

	const char* csource = source.c_str ();

 	glShaderSource (id, 1, &csource, NULL);
	glCompileShader (id);

	char error[1000];
	glGetShaderInfoLog (id, 1000, NULL, error);
	std::string stderror = error;
	Console::Log ("Compile status (empty means compile successfully) : " + stderror);

	return id;
}

int ShaderManager::AddShader (const std::string& shaderName,
	const std::string& vertexFile, const std::string& fragmentFile)
{
	if (_shaderCollection.find (shaderName) != _shaderCollection.end ()) {
		return 1;
	}

	unsigned int vertex, fragment, program;

	std::string source = "";
	LoadShaderFile (vertexFile, source);
	vertex = LoadShader (source, GL_VERTEX_SHADER);

	source = "";
	LoadShaderFile (fragmentFile, source);
	fragment = LoadShader (source, GL_FRAGMENT_SHADER);

	program = glCreateProgram ();
	glAttachShader (program, vertex);
	glAttachShader (program, fragment);

	glLinkProgram (program);

	_shaderCollection [shaderName] = program;
	_programShadersLink [program] = std::make_pair (vertex, fragment);

	return 0; 
}

int ShaderManager::DeleteShader (const std::string& shaderName)
{
	if (_shaderCollection.find (shaderName) == _shaderCollection.end ()) {
		return 1;
	}

	unsigned int program = _shaderCollection [shaderName];

	if (_programShadersLink.find (program) != _programShadersLink.end ()) {
		return -1;
	}

	std::pair<unsigned int, unsigned int> shaders = _programShadersLink [program];

	glDetachShader (program, shaders.first);
	glDetachShader (program, shaders.second);

	glDeleteShader (shaders.first);
	glDeleteShader (shaders.second);

	_programShadersLink.erase (program);

	glDeleteProgram (program);

	_shaderCollection.erase (shaderName);

	return 0;
}

unsigned int ShaderManager::GetShader (const std::string& shaderName)
{
	std::map<std::string, unsigned int>::iterator it = _shaderCollection.find (shaderName);

	if (it == _shaderCollection.end ()) {
		return 0;
	}

	return it->second;
}

void ShaderManager::Clear ()
{
	std::vector<std::string> collection;
	std::map<std::string, unsigned int>::iterator it = _shaderCollection.begin ();
	for (;it != _shaderCollection.end ();it++) {
		collection.push_back (it->first);
	}

	for (std::vector<std::string>::iterator itv = collection.begin ();itv < collection.end ();itv++) {
		DeleteShader (*itv);
	}

	_shaderCollection.clear ();
	_programShadersLink.clear ();
}