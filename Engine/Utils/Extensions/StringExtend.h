#ifndef STRINGEXTEND_H
#define STRINGEXTEND_H

#include <string>
#include <vector>

namespace Extensions
{

class StringExtend
{
public:
	static bool Replace (std::string&, const std::string&, const std::string&);
	static std::size_t ReplaceAll (std::string&, const std::string&, const std::string&);
	static std::size_t Trim (std::string&);
	static std::size_t Trim (std::string&, char ch);
	static std::size_t Trim (std::string&, const std::string& charset);

	static std::vector<std::string> Split (const std::string&, const std::string&);

	static bool ToBool (const std::string& boolString);
	static bool ToBoolExt (const std::string& boolString);

	static std::string Upper (const std::string&);
	static void ToUpper (std::string&);
	static std::string Lower (const std::string&);
	static void ToLower (std::string&);
};

}

#endif
