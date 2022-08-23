#ifndef __LEAF_CONFIG_H__
#define __LEAF_CONFIG_H__

#include <string>
#include <deque>
#include <stdint.h>

enum config_verbosity{
	CONFIG_V_DEFAULT = 0,
	CONFIG_V_VERBOSE = 1,
	CONFIG_V_SUPERVERBOSE = 2,
	CONFIG_V_ULTRAVERBOSE = 3
};

enum config_redownload{
	CONFIG_REDOWNLOAD_NONE = 0,
	CONFIG_REDOWNLOAD_SPECIFIED = 1,
	CONFIG_REDOWNLOAD_ALL = 2
};

enum leaf_action{
	ACTION_NONE = 0,
	ACTION_UPDATE = 1,
	ACTION_INSTALL = 2,
	ACTION_REMOVE = 3,
	ACTION_INSTALLLOCAL = 4
};

typedef struct leafconfig_struct{

	//The root directory leaf should work on (normally "/")
	std::string					rootDir = "/";

	//The action leaf 
	leaf_action					action = ACTION_NONE;

	//The packages that were submitted for lefa to process
	std::deque<std::string>		packages;

	//The level of verbosity leaf should use, 0 is the least verbose
	config_verbosity			verbosity = CONFIG_V_DEFAULT;
	
	//If leaf should redownload packages
	config_redownload			redownload = CONFIG_REDOWNLOAD_NONE;

	//If this flag is set, leaf will not ask for any permissions and just do the things assuming yes was chosen
	bool						noAsk = false;

	//If leaf should keep its package caches after a transaction
	bool						noClean = false;

	//If leaf should overwrite existing files at the root
	bool						forceOverwrite = false;

	//If leaf should run the preinstall scripts of the packages installed
	bool						runPreinstall = true;

	//If leaf should run the postinstall scripts of the packages installed
	bool						runPostinstall = true;

	//If leaf should install dependencies or if it should ignore them and proceed installing the package anyway
	bool						installDependencies = true;

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

	//Where the leaf configuration lives
	std::string					configDir(){
		return rootDir + "etc/leaf/";
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
		return configDir() + "leaf.pkglist";
	}

	//Set the root directory and append a '/' if needed
	void setRootDir(std::string dir){
		if (dir[dir.length()-1] != '/')
			dir += '/';
		rootDir = dir;
	}

} leaf_config_t;

//extern leaf_config_t lConfig;

#endif