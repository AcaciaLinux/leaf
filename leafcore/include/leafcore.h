#ifndef __LEAFCORE_H__
#define __LEAFCORE_H__

#include <string>
#include <vector>

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
	 * @brief	Installs the provided package with all of its dependencies
	 * @param	packages		The packages to process
	 */
	bool						a_install(std::vector<std::string> packages);

	/**
	 * @brief	Fetches the supplied package tarball into the working directory (wd/downloads)
	 * @param	package			The package to fetch
	 */
	bool						fetchPackage(Package* package);

	/**
	 * @brief	Extracts the supplied package
	 * @param	package			The package to process
	 */
	bool						extractPackage(Package* package);

	/**
	 * @brief	Deploys the supplied package to the specified _rootPath ("/")
	 * @param	package			The package to deploy
	 */
	bool						deployPackage(Package* package);

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

	//The directory leaf can work in
	std::string					_cachePath = "/var/cache/leaf/";

	//Where the currently loaded package list file is
	std::string					_pkglistFile = "/etc/leaf/leaf.pkglist";
	bool						_loadedPkgList = false;

	//The URL where to fetch the package list from
	std::string					_pkglistURL = "http://84.252.121.236/packages/leaf.pkglist";

	//The database of the available packages in the package list
	LeafDB						_packageListDB;

	//The database of the installed packages
	LeafDB						_installedDB;

	//Holds the last error in clear readable form
	std::string					_error;
};

#endif