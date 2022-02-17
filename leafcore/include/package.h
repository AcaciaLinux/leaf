#ifndef __PACKAGE_H__
#define __PACKAGE_H__

#include "file.h"

#include <string>
#include <deque>
#include <unordered_map>
#include <ostream>
#include <istream>

class Package{

public:
	Package(std::string name, std::string version);

	/**
	 * @brief	Adds the supplied file path to the supplied files by the package
	 * @param	filenPath		The path to the file relative to root (/) that the package provides
	 */
	void						addProvidedFile(std::string filenPath);

	/**
	 * @brief	Outputs the .leafinstalled file from the package
	 * @param	out				The output stream to write to
	 */
	bool						createInstalledFile(std::ostream& out);

	/**
	 * @brief	Parse a .leafinstalled file to this package, everything will be overwritten
	 * @param	in				The input stream to parse
	 */
	bool						parseInstalledFile(std::istream& in);

	/**
	 * @brief	Returns a reference to the files provided by this package
	 */
	std::deque<std::string>&	getProvidedFiles();

	/**
	 * @brief	Returns a reference to the dependencies needed by this package
	 */
	std::deque<std::string>&	getDependencies();

	/**
	 * @brief	The name of the package
	 */
	void						setName(std::string);
	std::string					getName();
	
	/**
	 * @brief	Returns the package name in the following format: "name-version":
	 * 			eg: "mypackage-12.31.3"
	 */
	std::string					getFullName();

	/**
	 * @brief	The version string of the package
	 */
	void						setVersion(std::string);
	std::string					getVersion();

	/**
	 * @brief	The description of the package
	 */
	void						setDescription(std::string);
	std::string					getDescription();

	/**
	 * @brief	The fetch URL the package can be received from
	 */
	void						setFetchURL(std::string);
	std::string					getFetchURL();

	/**
	 * @brief	Checks if the provided file is provided by the package
	 * @param	filePath		The filepath to the file being checked relative to root (/)
	 */
	bool						checkFileProvided(std::string filePath);

	/**
	 * @brief	Clears all the contents of this package
	 */
	void						clear();

	/**
	 * @brief	Returns the package described in a string 
	 */
	std::string					toString();

	/**
	 * @brief	Returns the last error thrown
	 */
	std::string					getError();

#ifndef FRIEND_PACKAGE
private:
#endif

	std::string					_error;

	std::string					_name;
	std::string					_versionString;
	std::string					_description;
	std::deque<std::string>		_dependencies;
	std::string					_fetchURL;
	
	std::deque<std::string>		_provided_files;
};

#endif
