#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>

class ENGINE_API FileSystem
{
public:
	static std::string GetDirectory(const std::string& filename);
	static std::string GetCurrentDirectory();

	static std::string GetExtension(const std::string& filename);

	static std::string FormatFilename (const std::string& filename);

	static std::string Relative (const std::string& filename, const std::string& relatedPath);
private:
	// TODO: reimplement this when implement platforming
	static std::string SwitchSlashesWindows (const std::string& filename);
	static std::string SwitchSlashesUNIX (const std::string& filename);
};

#endif