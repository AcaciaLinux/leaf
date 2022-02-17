#ifndef __LEAFCORE_H__
#define __LEAFCORE_H__

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
	 * @brief	Fetches the supplied package tarball into the working directory (wd/downloads)
	 * @param	package			The package to fetch
	 * @param	force			Force leaf to fetch the package, even if it is in the cache
	 */
	bool						fetchPackage(Package* package, bool force = false);

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
	 * @brief	Checks the leaf directories and prompts the user if something is wrong
	 */
	bool						checkDirectories();

	/**
	 * @brief	Asks the user for permission to do something in the format "question (y/n)"
	 * @param	question		The question to ask
	 * @param	defaultOption	If agreeing option should be checked or the negating option
	 */
	bool						askUserOK(std::string question, bool defaultOption = true);

	/**
	 * @brief	The root leaf should deploy packages to (normally "/")
	 */
	void						setRootDir(std::string);
	std::string					getRootDir();
	
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
	 * @brief	Returns the directory leaf is storing downloads
	 */
	std::string					getDownloadDir();

	/**
	 * @brief	Returns the directory leaf is extracting and caching its packages
	 */
	std::string					getPackagesDir();

	/**
	 * @brief	Get the path to the package download destination
	 * @param	package			The package to process
	 */
	std::string					getDownloadPath(Package* package);

	/**
	 * @brief	Get the path to the extracted package destination
	 */
	std::string					getExtractedDirectory(Package* package);

	/**
	 * @brief	Returns the last error
	 */
	std::string					getError();

private:
	//The path to apply leaf packages to (should be "/")
	std::string					_rootDir;

	//The directory leaf can work in
	std::string					_cacheDir;

	//The leaf configuration path
	std::string					_leafDir;

	//Where the currently loaded package list file is
	std::string					_pkglistFile;
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