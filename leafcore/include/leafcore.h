#ifndef __LEAFCORE_H__
#define __LEAFCORE_H__

class Leafcore;

#include <string>
#include <list>
#include <deque>
#include <filesystem>

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
	 * @brief	Creates a new leaf instance with the supplied config
	 * @param	conf			The config to use
	 */
	Leafcore(leaf_config_t conf);

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
	 * @brief	Upgrades the supplied packages
	 * @param	packages		The packages to install
	 */
	void						a_upgrade(std::deque<std::string> packages);

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
	bool						askUserOK(std::string question, bool defaultOption = true) const;

	/**
	 * @brief	Sets the config this Leafcore instance will use
	 * @param	config			The config to apply
	 */
	void						setConfig(leaf_config_t config);

	/**
	 * @brief	Get a reference to the current leaf config
	 * @return	leaf_config_t&
	 */
	leaf_config_t&				getConfig();

	/**
	 * @brief	Returns a copy of the currently loaded configuration
	 * @return	leaf_config_t
	 */
	leaf_config_t				getConfigC() const;

	/**
	 * @brief	Executes a command, chroots into another root if necessary
	 * @param	command			The command to execute
	 * @param	workdir			The directory the command should be executed in
	 * @return	The commands return value
	 */
	int							runCommand(const std::string& command, const std::filesystem::path& workdir) const;

#ifndef FRIEND_LEAFCORE
private:
#endif

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

	///
	///	Some utility functions
	///

	/**
	 * @brief	Downlads the supplied list of packages into the download cache
	 * @param	packages		The packages to process
	 */
	void						download_packages(const std::deque<Package*>& packages);

	/**
	 * @brief	Checks the supplied list of packages for their validity using the MD5 hash
	 * @param	packages		The packages to check
	 */
	void						check_pkg_hashes(const std::deque<Package*>& packages);

	/**
	 * @brief	Installs the supplied list of packages into the system
	 * @param	packages		The packages to install
	 */
	void						install_packages(const std::deque<Package*>& packages);

};

#endif