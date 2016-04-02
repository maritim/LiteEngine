#include "ShaderManager.h"

#include <GL/glew.h>
#include <string>
#include <map>
#include <fstream>

#include "Core/Console/Console.h"

#include "Wrappers/OpenGL/GL.h"

std::map<std::string, Shader*> ShaderManager::_shaderCollection;

// medium priority TODO: Improve this
void ShaderManager::LoadShaderFile (const std::string& filename, std::string& content)
{
	std::ifstream f (filename);

	if (!f.is_open ()) {
		Console::LogError ("Shader " + filename + " could not be opened!");

		return ;
	}

	std::string line;

	while (!f.eof ()) {
		std::getline (f, line);
		content += line;
		content += "\n";
	}

	f.close ();
}

unsigned int ShaderManager::LoadShader (const std::string& source, unsigned int mode)
{
	unsigned int id;
	id = GL::CreateShader (mode);

	const char* csource = source.c_str ();

 	GL::ShaderSource (id, 1, &csource, NULL);
	GL::CompileShader (id);

	ErrorCheck (id);

	return id;
}

int ShaderManager::AddShader (const std::string& shaderName,
	const std::string& vertexFile, const std::string& fragmentFile)
{
	if (_shaderCollection.find (shaderName) != _shaderCollection.end ()) {
		return 1;
	}

	unsigned int vertex, fragment, program;
	std::string source;

	source = "";
	LoadShaderFile (vertexFile, source);
	vertex = LoadShader (source, GL_VERTEX_SHADER);

	source = "";
	LoadShaderFile (fragmentFile, source);
	fragment = LoadShader (source, GL_FRAGMENT_SHADER);

	program = GL::CreateProgram ();
	GL::AttachShader (program, vertex);
	GL::AttachShader (program, fragment);

	GL::LinkProgram (program);

	Shader* shader = new Shader (shaderName, program, vertex, fragment);
	shader->SetVertexFilename (vertexFile);
	shader->SetFragmentFilename (fragmentFile);

	_shaderCollection [shaderName] = shader;

	return 0; 
}

int ShaderManager::DeleteShader (const std::string& shaderName)
{
	if (_shaderCollection.find (shaderName) == _shaderCollection.end ()) {
		return 1;
	}

	Shader* shader = _shaderCollection [shaderName];

	GL::DetachShader (shader->GetProgram (), shader->GetVertexShader ());
	GL::DetachShader (shader->GetProgram (), shader->GetFragmentShader ());

	GL::DeleteShader (shader->GetVertexShader ());
	GL::DeleteShader (shader->GetFragmentShader ());

	GL::DeleteProgram (shader->GetProgram ());

	_shaderCollection.erase (shaderName);

	return 0;
}

Shader* ShaderManager::GetShader (const std::string& shaderName)
{
	std::map<std::string, Shader*>::iterator it = _shaderCollection.find (shaderName);

	if (it == _shaderCollection.end ()) {
		return nullptr;
	}

	return it->second;
}

void ShaderManager::Clear ()
{
	std::map<std::string, Shader*>::iterator it = _shaderCollection.begin ();
	for (;it != _shaderCollection.end ();it++) {
		DeleteShader (it->first);
	}

	_shaderCollection.clear ();
}

void ShaderManager::ErrorCheck (unsigned int shader)
{
	char *error = (char*) malloc (10000);

	GL::GetShaderInfoLog (shader, 10000, NULL, error);

	Console::Log ("Compile status (empty means compiles successfully) : " + (std::string) error);

	delete error;
}