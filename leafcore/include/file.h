#ifndef __BINARY_H__
#define __BINARY_H__

class File;

#include "package.h"

#include <string>

/**
 * @brief	This describes a file of a package using its name and path
 * 			Every file has a provider, which is a package that provides this file
 * 			Every file in a package gets represented using this class
 */
class File{

public:
	File(std::string name, std::string path, std::string provider);

	/**
	 * @brief	Returns the name of the package
	 */
	std::string				getName();

	/**
	 * @brief	Returns the path of the file relative to the
	 * 			root of the file system e.g.: "etc/myfile.conf", altrough
	 * 			it does not include a "/" at the beginning, this is to allow changing the package root
	 */
	std::string				getPath();

private:
	std::string				_name;
	std::string				_path;
	
	std::string				_provider;
};

#endif