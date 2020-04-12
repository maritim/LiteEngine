#ifndef HEADERPROCESSOR_H
#define HEADERPROCESSOR_H

#include <vector>
#include <string>

#include "Lexer.h"

struct AttributeProduct;

struct ClassProduct
{
	std::string TypeName;
	std::vector<std::string> ParentNames;
	std::vector<AttributeProduct> Attributes;
};

struct AttributeProduct
{
	std::string TypeName;
	std::string Name;
};

class HeaderProcessor
{
public:
	ClassProduct Process (const std::string& filename);
protected:
	ClassProduct ProcessClass (Lexer& lexer);
	AttributeProduct ProcessAttribute (Lexer& lexer);
	std::string ProcessTypeName (Lexer& lexer, Token& token);
};

#endif