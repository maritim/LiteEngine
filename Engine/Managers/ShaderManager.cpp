#include "ShaderManager.h"

#include "Core/Console/Console.h"

#include "Wrappers/OpenGL/GL.h"

#include "Resources/Resources.h"

ShaderManager::ShaderManager ()
{

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

void ShaderManager::LoadShaderFile (const std::string& filename, std::string& content)
{
	ShaderContent* shaderContent = Resources::LoadShaderContent (filename);

	content = shaderContent->GetContent ();

	delete shaderContent;
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

GLuint ShaderManager::AddShader (const std::string& shaderName,
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

	GLuint program = GL::CreateProgram ();

	/*
	 * Load, compile and attach vertex shader
	*/

	GLuint vertex = BuildShaderFile (program, GL_VERTEX_SHADER, vertexFile);

	/*
	 * Load, compile and attach fragment shader
	*/

	GLuint fragment = BuildShaderFile (program, GL_FRAGMENT_SHADER, fragmentFile);

	/*
	 * Load, compile and attach geometry shader if exists
	*/

	GLuint geometry = 0;

	if (geometryFile != "") {
		geometry = BuildShaderFile (program, GL_GEOMETRY_SHADER, geometryFile);
	}

	GL::LinkProgram (program);

	DrawingShader* shader = new DrawingShader (shaderName, program, vertex, fragment, geometry);
	shader->SetVertexFilename (vertexFile);
	shader->SetFragmentFilename (fragmentFile);
	shader->SetGeometryFilename (geometryFile);

	_shaderCollection [shaderName] = shader;

	return program; 
}

GLuint ShaderManager::AddComputeShader (const std::string& shaderName, const std::string& computeFile)
{
	/*
	* Return the program if already exists
	*/

	auto it = _shaderCollection.find(shaderName);
	if (it != _shaderCollection.end()) {
		return it->second->GetProgram();
	}

	GLuint program = GL::CreateProgram();

	/*
	* Load, compile and attach vertex shader
	*/

	GLuint compute = BuildShaderFile (program, GL_COMPUTE_SHADER, computeFile);

	GL::LinkProgram(program);

	ComputeShader* shader = new ComputeShader (shaderName, program, compute);
	shader->SetComputeFilename(computeFile);

	_shaderCollection[shaderName] = (Shader*) shader;

	return program;
}

int ShaderManager::DeleteShader (const std::string& shaderName)
{
	if (_shaderCollection.find (shaderName) == _shaderCollection.end ()) {
		return 1;
	}

	/*
	 * Find shader
	*/

	Shader* shader = _shaderCollection [shaderName];

	/*
	 * Remove shader from managed collection
	*/

	_shaderCollection.erase (shaderName);

	/*
	 * Delete shader from memory
	*/

	delete shader;

	return 0;
}

Shader* ShaderManager::GetShader (const std::string& shaderName)
{
	auto iterrator = _shaderCollection.find (shaderName);

	if (iterrator == _shaderCollection.end ()) {
		return nullptr;
	}

	return iterrator->second;
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

	free (error);
}