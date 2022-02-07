#ifndef __LEAFCORE_H__
#define __LEAFCORE_H__

#include <string>

#include "leafdb.h"

/**
 * @brief	The core leaf interface
 */
class Leafcore{

public:
	/**
	 * @brief	Create the leaf core context and set the root path
	 * @param	rootPath		The root leaf should deploy packages to (normally "/")
	 */
	Leafcore(std::string rootPath);	

	/**
	 * @brief	Reads the package list at the supplied path
	 * @param	path			The full path to the package list
	 */
	bool						parsePackageList(std::string path);
	bool						parsePackageList();

	/**
	 * @brief	Updates the local package list
	 */
	bool						a_update();

	/**
	 * @brief	The root leaf should deploy packages to (normally "/")
	 */
	void						setRootPath(std::string);
	std::string					getRootPath();
	
	/**
	 * @brief	The path to the package list file (has default)
	 */
	void						setPkgListFile(std::string);
	std::string					getPkgListFile();

	/**
	 * @brief	The URL to fetch the package list from (has default)
	 */
	void						setPkgListURL(std::string);
	std::string					getPkgListURL();

	/**
	 * @brief	Returns the last error
	 */
	std::string					getError();

private:
	//The path to apply leaf packages to (should be "/")
	std::string					_rootPath;

	//Where the currently loaded package list file is
	std::string					_pkglistFile = "packages.list";

	//The URL where to fetch the package list from
	std::string					_pkglistURL = "https://raw.githubusercontent.com/AcaciaLinux/leaf_packages/main/packages.list";

	//The main database leaf works with
	LeafDB						_db;

	//Holds the last error in clear readable form
	std::string					_error;
};

#endif