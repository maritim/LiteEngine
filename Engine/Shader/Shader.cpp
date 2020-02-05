#include "Shader.h"

Shader::Shader (const std::string& name) :
	_name(name)
{

}

std::string Shader::GetName () const
{
	return _name;
}
