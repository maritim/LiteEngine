#include "FileSystem.h"

#include <string>
#include <algorithm>

#include "Utils/Extensions/StringExtend.h"

// TODO: Implement this
std::string FileSystem::GetCurrentDirectory()
{
	return "";
}

std::string FileSystem::GetDirectory(const std::string& filename)
{
	std::string directory = "";
	std::size_t directoryEndIndex = filename.size ();

	while (directoryEndIndex >= 0 && filename[directoryEndIndex] != '/') {
		--directoryEndIndex;
	}

	for (std::size_t i=0; i <= directoryEndIndex;i++) {
		directory += filename [i];
	}

	return directory;
}

std::string FileSystem::GetExtension(const std::string& filename)
{
	std::string extension = "";
	std::size_t iterator = filename.size() - 1;

	while (iterator >= 0 && filename[iterator]!='.') {
		extension += filename [iterator];
		iterator --;
	}

	extension += ".";

	std::reverse (extension.begin(), extension.end());

	return extension;
}

std::string FileSystem::FormatFilename (const std::string& filename)
{
	std::string formated = filename;

	Extensions::StringExtend::Trim (formated);

	formated = SwitchSlashesUNIX (formated);

	Extensions::StringExtend::ReplaceAll (formated, "//", "/");

	return formated;
}

std::string FileSystem::Relative (const std::string& filename, const std::string& relatedPath)
{
	std::string formated = filename;

	Extensions::StringExtend::Replace (formated, relatedPath, "");

	Extensions::StringExtend::Trim (formated, '/');

	return formated;
}

// TODO: Implement this
std::string FileSystem::SwitchSlashesWindows (const std::string& filename)
{
	return "";
}

std::string FileSystem::SwitchSlashesUNIX (const std::string& filename)
{
	std::string formated = filename;

	for (std::size_t i=0;i<formated.length ();i++) {
		if (formated [i] == '\\') {
			formated [i] = '/';
		}
	}

	return formated;
}