#ifndef __LEAFCORE_H__
#define __LEAFCORE_H__

class Leafcore;

#include <string>
#include <list>
#include <deque>

#include "leafdb.h"
#include "parser.h"
#include "leafconfig.h"
#include "hook.h"
#include "branchmaster.h"

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
	 * @brief	The destructor for the leafcore
	 */
	~Leafcore();

	/**
	 * @brief	Parses the leaf config from the predetermined path
	 */
	void						parseConfig();

	/**
	 * @brief	Reads the package list at the supplied path
	 * @param	path			The full path to the package list
	 */
	void						parsePackageList(std::string path);
	void						parsePackageList();

	/**
	 * @brief	Parses all the installed packages into the leaf database
	 */
	void						parseInstalled();

	/**
	 * @brief	Finds and parses leaf hooks in the $ROOT/etc/leaf/hooks directory
	 */
	void						parseHooks();

	/**
	 * @brief	Updates the local package list
	 */
	void						a_update();

	/**
	 * @brief	Installs the provided package with all of its dependencies
	 * @param	packages		The packages to install
	 */
	void						a_install(std::deque<std::string> packages);

	/**
	 * @brief	Installs the provided local leaf packages
	 * @param	packages		The packages to install
	 */
	void						a_installLocal(std::deque<std::string> packages);

	/**
	 * @brief	Removes the specified packages from the system
	 * @param	packages		The packages to remove 
	 */
	void						a_remove(std::deque<std::string> packages);

	/**
	 * @brief	Checks the leaf directories and prompts the user if something is wrong
	 */
	void						checkDirectories();

	/**
	 * @brief	Creates all the cache directories
	 */
	void						createCacheDirs();

	/**
	 * @brief	Creates all the configuration directories
	 */
	void						createConfigDirs();

	/**
	 * @brief	Asks the user for permission to do something in the format "question (y/n)"
	 * @param	question		The question to ask
	 * @param	defaultOption	If agreeing option should be checked or the negating option
	 */
	bool						askUserOK(std::string question, bool defaultOption = true);

	/**
	 * @brief	Sets the config this Leafcore instance will use
	 * @param	config			The config to apply
	 */
	void						setConfig(leaf_config_t config);

	/**
	 * @brief	Get a reference to the current leaf config
	 * @return	leaf_config_t
	 */
	leaf_config_t&				getConfig();

private:

	//Where the currently loaded package list file is
	bool						_loadedPkgList = false;

	//The database of the available packages in the package list
	LeafDB*						_packageListDB;

	//The database of the installed packages
	LeafDB*						_installedDB;

	//The config parser for the main leaf configuration
	Parser						_configParser;

	//All the hooks leafcore has found
	std::deque<Hook>			_hooks;

	//The current leaf config to use
	leaf_config_t				_config;
};

#endif