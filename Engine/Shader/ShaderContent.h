#ifndef SHADERCONTENT_H
#define SHADERCONTENT_H

#include "Core/Interfaces/Object.h"

class ShaderContent : public Object
{
protected:
	std::string _filename;
	std::string _content;

public:
	void SetFilename (const std::string& filename);
	void SetContent (const std::string& content);

	const std::string& GetFilename () const;
	const std::string& GetContent () const;
};

#endif