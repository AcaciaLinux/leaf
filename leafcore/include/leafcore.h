#ifndef __LEAFCORE_H__
#define __LEAFCORE_H__

class Leafcore;

#include <string>
#include <list>

#include "leafdb.h"

/**
 * @brief	The core leaf interface
 */
class Leafcore{

public:
	/**
	 * @brief	Create the leaf core context and set the root path
	 */
	Leafcore();

	/**
	 * @brief	Reads the package list at the supplied path
	 * @param	path			The full path to the package list
	 */
	bool						parsePackageList(std::string path);
	bool						parsePackageList();

	/**
	 * @brief	Parses all the installed packages into the leaf database
	 */
	bool						parseInstalled();

	/**
	 * @brief	Updates the local package list
	 */
	bool						a_update();

	/**
	 * @brief	Installs the provided package with all of its dependencies
	 * @param	packages		The packages to process
	 * @param	force			Force leaf to fetch the package, even if it is in the cache
	 */
	bool						a_install(std::deque<std::string> packages, bool force = false);

	/**
	 * @brief	Deploys the supplied package to the specified _rootPath ("/")
	 * @param	package			The package to deploy
	 */
	bool						deployPackage(Package* package);

	/**
	 * @brief	Copies all the data of the package to the root
	 * @param	package			The package to copy the data of
	 */
	bool						copyDataToRoot(Package* package);

	/**
	 * @brief	Executes the preinstall script of a package if it exists
	 * @param	package			The package to run the script of
	 */
	bool						runPreInstall(Package* package);

	/**
	 * @brief	Executes the postinstall script of a package if it exists
	 * @param	package			The package to run the script of
	 */
	bool						runPostInstall(Package* package);

	/**
	 * @brief	Checks the leaf directories and prompts the user if something is wrong
	 */
	bool						checkDirectories();

	/**
	 * @brief	Creates all the cache directories
	 */
	bool						createCacheDirs();

	/**
	 * @brief	Asks the user for permission to do something in the format "question (y/n)"
	 * @param	question		The question to ask
	 * @param	defaultOption	If agreeing option should be checked or the negating option
	 */
	bool						askUserOK(std::string question, bool defaultOption = true);

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

	//Where the currently loaded package list file is
	bool						_loadedPkgList = false;

	//The URL where to fetch the package list from
	std::string					_pkglistURL = "http://84.252.121.236/packages/leaf.pkglist";

	//The database of the available packages in the package list
	LeafDB*						_packageListDB;

	//The database of the installed packages
	LeafDB*						_installedDB;

	//Holds the last error in clear readable form
	std::string					_error;
};

#endif