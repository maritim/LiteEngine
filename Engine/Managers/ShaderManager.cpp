#include "ShaderManager.h"

#include <GL/glew.h>
#include <string>
#include <map>
#include <fstream>

#include "Core/Console/Console.h"

#include "Wrappers/OpenGL/GL.h"

ShaderManager::ShaderManager ()
{
	// AddShader ("DEFAULT", "Assets/Shaders/defaultVertex.glsl", "Assets/Shaders/defaultFragment.glsl");	
	AddShader ("DEFAULT", "Assets/Shaders/deferredVertex.glsl", "Assets/Shaders/deferredFragment.glsl", "Assets/Shaders/deferredGeometry.glsl");
	AddShader ("DEFAULT_ANIMATED", "Assets/Shaders/deferredVertexAnimation.glsl", "Assets/Shaders/deferredFragment.glsl", "Assets/Shaders/deferredGeometry.glsl");
}

ShaderManager::~ShaderManager ()
{
	Clear();
}

/*
 * Load shader file, compile it based on type (vertex, geometry, fragment), attach
 * the shader to respective program.
*/

unsigned int ShaderManager::BuildShaderFile (unsigned int program, int shaderType, const std::string& filename)
{
	unsigned int shader;
	std::string source;

	Console::Log ("Loading and compiling \"" + filename + "\" !");

	LoadShaderFile (filename, source);
	shader = LoadShader (source, (GLenum)shaderType);

	GL::AttachShader (program, shader);	

	return shader;
}

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
	const std::string& vertexFile, const std::string& fragmentFile,
	const std::string& geometryFile)
{
	/*
	 * Return the program if already exists
	*/

	auto it = _shaderCollection.find (shaderName);
	if (it != _shaderCollection.end ()) {
		return it->second->GetProgram ();
	}

	unsigned int program = GL::CreateProgram ();

	/*
	 * Load, compile and attach vertex shader
	*/

	unsigned int vertex = BuildShaderFile (program, GL_VERTEX_SHADER, vertexFile);

	/*
	 * Load, compile and attach fragment shader
	*/

	unsigned int fragment = BuildShaderFile (program, GL_FRAGMENT_SHADER, fragmentFile);

	/*
	 * Load, compile and attach geometry shader if exists
	*/

	unsigned int geometry = 0;

	if (geometryFile != "") {
		geometry = BuildShaderFile (program, GL_GEOMETRY_SHADER, geometryFile);
	}

	GL::LinkProgram (program);

	Shader* shader = new Shader (shaderName, program, vertex, fragment, geometry);
	shader->SetVertexFilename (vertexFile);
	shader->SetFragmentFilename (fragmentFile);
	shader->SetGeometryFilename (geometryFile);

	_shaderCollection [shaderName] = shader;

	return program; 
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

void ShaderManager::Clear()
{
	while (_shaderCollection.size() != 0) {
		DeleteShader(_shaderCollection.begin()->first);
	}
}

void ShaderManager::ErrorCheck (unsigned int shader)
{
	char *error = (char*) malloc (10000);

	GL::GetShaderInfoLog (shader, 10000, NULL, error);

	Console::Log ("Compile status (empty means compiles successfully) : " + (std::string) error);

	delete error;
}