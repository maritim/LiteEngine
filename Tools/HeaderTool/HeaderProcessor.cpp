#include "HeaderProcessor.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

ClassProduct HeaderProcessor::Process (const std::string& filename)
{
	std::ifstream file(filename.c_str ());

    std::stringstream strStream;
    strStream << file.rdbuf(); //read the file
    std::string text = strStream.str(); //str holds the content of the file

	Lexer lex (text.c_str ());

	auto token = lex.next ();

	while ( !token.is_one_of (Token::Kind::End, Token::Kind::Unexpected)) {

		if (token.is (Token::Kind::Identifier) && token.lexeme () == "class") {
			return ProcessClass (lex);
		}

		token = lex.next ();
	}

	return ClassProduct ();
}

ClassProduct HeaderProcessor::ProcessClass (Lexer& lexer)
{
	ClassProduct classProduct;

	auto token = lexer.next ();

	if (token.lexeme () == "ENGINE_API") {
		token = lexer.next ();
	}

	classProduct.TypeName = token.lexeme ();

	while (token.lexeme () != "{") {
		while ((!(token = lexer.next ()).is (Token::Kind::Identifier)
				|| token.lexeme () == "public"
				|| token.lexeme () == "private"
				|| token.lexeme () == "protected")
			&& token.lexeme () != "{");

		if (token.is (Token::Kind::Identifier)) {
			classProduct.ParentNames.push_back (token.lexeme ());
		}
	}

	std::size_t depth = 1;

	while (depth > 0) {

		token = lexer.next ();

		if (token.lexeme () == "{") {
			depth ++;
		}

		if (token.lexeme () == "}") {
			depth --;
		}

		if (token.lexeme () == "ATTRIBUTE") {
			classProduct.Attributes.push_back (ProcessAttribute (lexer));
		}
	}

	return classProduct;
}

AttributeProduct HeaderProcessor::ProcessAttribute (Lexer& lexer)
{
	AttributeProduct attribute;

	auto token = lexer.next ();
	
	std::size_t depth = 1;

	while (depth > 0) {
		token = lexer.next ();

		if (token.lexeme () == "(") {
			depth ++;
		}

		if (token.lexeme () == ")") {
			depth --;
		}
	}

	attribute.TypeName = ProcessTypeName (lexer, token);

	while (!token.is (Token::Kind::Identifier)) {
		token = lexer.next ();
	}

	attribute.Name = token.lexeme ();

	return attribute;
}

std::string HeaderProcessor::ProcessTypeName (Lexer& lexer, Token& token)
{
	std::string typeName;

	token = lexer.next ();

	while (!token.is (Token::Kind::Identifier)) {
		token = lexer.next ();
	}

	typeName += token.lexeme ();

	token = lexer.next ();

	if (token.lexeme () == ":") {
		lexer.next ();
		typeName += "::";
		typeName += ProcessTypeName (lexer, token);
	}

	if (token.lexeme () == "*") {
		typeName += "*";
	}

	if (token.lexeme () != "<") {
		return typeName;
	}

	typeName += "<";

	std::size_t depth = 1;

	while (depth > 0) {

		token = lexer.next ();

		if (token.lexeme () == "<") {
			depth ++;
		}

		if (token.lexeme () == ">") {
			depth --;
		}

		typeName += token.lexeme ();
	}

	return typeName;
}
