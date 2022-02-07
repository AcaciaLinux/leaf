#ifndef __PACKAGE_H__
#define __PACKAGE_H__

#include "file.h"

#include <string>
#include <vector>
#include <unordered_map>

class Package{

public:
	Package(std::string name);

	/**
	 * @brief	Returns the name of the package
	 */
	std::string					getName();

	/**
	 * @brief	Returns the description of the package
	 */
	std::string					getDescription();

	/**
	 * @brief	Adds the supplied file path to the supplied files by the package
	 * @param	filenPath		The path to the file relative to root (/) that the package provides
	 */
	void						addProvidedFile(std::string filenPath);

	/**
	 * @brief	Returns a reference to the files provided by this package
	 * @return	std::vector<std::string>&
	 */
	std::vector<std::string>&	getProvidedFiles();

	/**
	 * @brief	Returns a reference to the dependencies needed by this package
	 * @return	std::vector<std::string>& 
	 */
	std::vector<std::string>&	getDependencies();

	/**
	 * @brief	Checks if the provided file is provided by the package
	 * @param	filePath		The filepath to the file being checked relative to root (/)
	 */
	bool						checkFileProvided(std::string filePath);

	/**
	 * @brief	Returns the package described in a string 
	 */
	std::string					toString();

#ifndef FRIEND_PACKAGE
private:
#endif

	std::string					_name;
	std::string					_description;
	std::vector<std::string>	_dependencies;
	std::string					_fetchURL;
	
	std::vector<std::string>	_provided_files;
};

#endif
