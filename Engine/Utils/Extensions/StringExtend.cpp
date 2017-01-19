// Source: http://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string

#include "StringExtend.h"

#include <cctype>
#include <algorithm>

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
    while (start <= end && std::isspace (source [start])) {
        ++ start;
    }

    while (start <= end && std::isspace (source [end])) {
        -- end;
    }

    std::size_t whitespaceCnt = start + source.length () - 1 - end;
    source = source.substr (start, source.length () - whitespaceCnt);

    return whitespaceCnt;
}

std::size_t StringExtend::Trim (std::string& source, char ch)
{
    std::size_t start = 0, end = source.length () - 1;

    // Is whitespace
    while (start <= end && source [start] == ch) {
        ++ start;
    }

    while (start <= end && source [end] == ch) {
        -- end;
    }

    std::size_t whitespaceCnt = start + source.length () - 1 - end;
    source = source.substr (start, source.length () - whitespaceCnt);

    return whitespaceCnt;    
}

std::vector<std::string> StringExtend::Split (
    const std::string& source, const std::string& charset)
{
    std::vector<std::string> result;
    std::vector<bool> check (256, false);
    std::string aux;

    for (char ch : charset) {
        check [(std::size_t) ch] = true;
    }

    for (std::size_t i=0;i<source.size ();i++) {
        if (check [(std::size_t) source [i]]) {
            result.push_back (aux);
            aux.clear ();
        } else {
            aux += source [i];
        }
    }

    if (aux.size () > 0) {
        result.push_back (aux);
    }

    return result;
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

bool StringExtend::ToBoolExt (const std::string& boolStr)
{
    std::string str = Upper (boolStr);

    if (boolStr == "FALSE" || boolStr == "0") {
        return false;
    }

    if (boolStr == "TRUE") {
        return true;
    }

    return true;
}

void StringExtend::ToUpper (std::string& source)
{
    std::transform (source.begin (), source.end (), source.begin (), ::toupper);
}

void StringExtend::ToLower (std::string& source)
{
    std::transform (source.begin (), source.end (), source.begin (), ::toupper);
}

std::string StringExtend::Upper (const std::string& source)
{
    std::string result = source;
    ToUpper (result);

    return result;
}

std::string StringExtend::Lower (const std::string& source)
{
    std::string result = source;
    ToLower (result);

    return result;
}
