#include "ShaderContent.h"

void ShaderContent::SetFilename (const std::string& filename)
{
	_filename = filename;
}

void ShaderContent::SetContent (const std::string& content)
{
	_content = content;
}

const std::string& ShaderContent::GetFilename () const
{
	return _filename;
}

const std::string& ShaderContent::GetContent () const
{
	return _content;
}
