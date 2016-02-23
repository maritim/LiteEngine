#ifndef STRINGEXTEND_H
#define STRINGEXTEND_H

#include <string>

namespace Extensions
{

class StringExtend
{
public:
	static bool Replace (std::string&, const std::string&, const std::string&);
	static std::size_t ReplaceAll (std::string&, const std::string&, const std::string&);
	static std::size_t Trim (std::string&);

	static bool ToBool (const std::string& boolString);
};

}

#endif
