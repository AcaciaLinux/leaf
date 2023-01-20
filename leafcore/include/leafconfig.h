#ifndef __LEAF_CONFIG_H__
#define __LEAF_CONFIG_H__

#include <string>
#include <deque>
#include <stdint.h>

#ifndef LEAF_DEFAULT_NOASK
	#define LEAF_DEFAULT_NOASK false
#endif

enum config_verbosity{
	CONFIG_V_DEFAULT = 0,
	CONFIG_V_VERBOSE = 1,
	CONFIG_V_SUPERVERBOSE = 2,
	CONFIG_V_ULTRAVERBOSE = 3,

	COUNT_CONFIG_V
};

enum config_redownload{
	CONFIG_REDOWNLOAD_NONE = 0,
	CONFIG_REDOWNLOAD_SPECIFIED = 1,
	CONFIG_REDOWNLOAD_ALL = 2,

	COUNT_CONFIG_REDOWNLOAD
};

enum leaf_action{
	ACTION_NONE = 0,
	ACTION_UPDATE = 1,
	ACTION_INSTALL = 2,
	ACTION_REMOVE = 3,
	ACTION_INSTALLLOCAL = 4,
	ACTION_UPGRADE = 5,

	COUNT_CONFIG_ACTION
};

typedef struct leafconfig_struct{

	//The root directory leaf should work on (normally "/")
	//If we are in a testing file, redirect this path to a safe location
	#ifdef LEAF_TESTING
		std::string					rootDir = LEAF_TESTING_ROOTDIR;
	#else
		std::string					rootDir = "/";
	#endif

	//The URL for fetching the main package list
	std::string					pkgListURL = "https://api.acacialinux.org/?get=packagelist";

	//The chrooting command leaf uses
	std::string					chroot_cmd = "chroot {ROOTDIR} {COMMAND}";

	//The action leaf 
	leaf_action					action = ACTION_NONE;

	//The packages that were submitted for lefa to process
	std::deque<std::string>		packages;

	//The level of verbosity leaf should use, 0 is the least verbose
	config_verbosity			verbosity = CONFIG_V_DEFAULT;
	
	//If leaf should redownload packages
	config_redownload			redownload = CONFIG_REDOWNLOAD_NONE;

	//If this flag is set, leaf will not ask for any permissions and just do the things assuming yes was chosen
	bool						noAsk = LEAF_DEFAULT_NOASK;

	//If leaf should keep its package caches after a transaction
	bool						noClean = false;

	//Disables the display of a beautiful progress bar
	bool						noProgress = false;

	//Forces certain safety features off
	bool						force = false;

	//If leaf should overwrite existing files at the root
	bool						forceOverwrite = false;

	//If leaf should run the preinstall scripts of the packages installed
	bool						runPreinstall = true;

	//If leaf should run the postinstall scripts of the packages installed
	bool						runPostinstall = true;

	//If leaf should install dependencies or if it should ignore them and proceed installing the package anyway
	bool						installDependencies = true;

	//If leaf should check the remote hash against the installed and decide if the package has an upgrade available
	bool						checkRemoteHashUpgrade = false;

	//The directory leaf can do its temporary work in
	std::string					cacheDir(){
		return rootDir + "var/cache/leaf/";
	}

	//The directory packages get downloaded to
	std::string					downloadDir(){
		return cacheDir() + "downloads/";
	}

	//The directory extracted packages live in
	std::string					packagesDir(){
		return cacheDir() + "packages/";
	}

	std::string					runScriptsDir(){
		return cacheDir() + "runScripts/";
	}

	//Where the leaf configuration lives
	std::string					configDir(){
		return rootDir + "etc/leaf/";
	}

	//Where the leaf configuration file lives
	std::string					configFile(){
		return configDir() + "leaf.conf";
	}

	//The directory leaf stores the installed packages information
	std::string					installedDir(){
		return configDir() + "installed/";
	}

	//The directory where all the leaf hooks are stored
	std::string					hooksDir(){
		return configDir() + "hooks/";
	}

	//The path to the package list
	std::string					pkgListPath(){
		return configDir() + "pkglist.json";
	}

	//Set the root directory and append a '/' if needed
	void setRootDir(std::string dir){
		if (dir[dir.length()-1] != '/')
			dir += '/';
		rootDir = dir;
	}

} leaf_config_t;

#endif
