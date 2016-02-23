// Source: http://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string

#include "StringExtend.h"

#include <string>
#include <locale>

#include "Core/Console/Console.h"

using namespace Extensions;

// TODO: reimplement this

bool StringExtend::Replace (std::string& source, const std::string& prototype, const std::string& copy)
{
    std::size_t startpos = source.find(prototype);

    if(startpos == std::string::npos) {
        return false;
    }

    source.replace(startpos, prototype.size (), copy);
    return true;
}

std::size_t StringExtend::ReplaceAll (std::string& source, const std::string& prototype, const std::string& copy)
{
	if(prototype.empty()) {
        return 0;
    }

    std::size_t startpos = 0, counter = 0;
    while((startpos = source.find(prototype, startpos)) != std::string::npos) {
        source.replace(startpos, prototype.length(), copy);
        startpos += copy.length();
        counter ++;
    }

    return counter;
}

std::size_t StringExtend::Trim (std::string& source)
{
    std::size_t start = 0, end = source.length () - 1;

    // Is whitespace
    while (std::isspace (source [start])) {
        ++ start;
    }

    while (std::isspace (source [end])) {
        -- end;
    }

    std::size_t whitespaceCnt = start + source.length () - 1 - end;
    source = source.substr (start, source.length () - whitespaceCnt);

    return whitespaceCnt;
}

bool StringExtend::ToBool (const std::string& boolStr)
{
    if (boolStr == "true") {
        return true;
    }

    if (boolStr == "false") {
        return false;
    }

    Console::LogWarning ("String " + boolStr + " cannot be converted to bool!");

    return false;
}