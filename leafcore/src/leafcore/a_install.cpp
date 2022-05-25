/**
 * @file		leafcore/a_install.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Leafcore::a_install()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"

#include <filesystem>

void Leafcore::a_install(std::deque<std::string> packages){
	FUN();
	LEAF_DEBUG_EX("Leafcore::a_install()");

	checkDirectories();

	//Check if the package list is loaded
	if (!_loadedPkgList)
		throw new LeafError(Error::PKGLIST_NOTLOADED);

	//Check if there even are some packages to process
	if (packages.size() == 0)
		throw new LeafError(Error::NOPACKAGE);

	{//Inform the user about the action
		std::string msg = "Installing following packages:";
		for (std::string pkg : packages)
			msg += " " + pkg;
		LOGU(msg);
	}

	std::deque<Package*> install_packages;
	LOGU("Resolving dependencies...");
	for (std::string packageName : packages){
		//This throws an error if the package was not found
		Package* package = _packageListDB->getPackage(packageName, true);

		//Resolve the dependencies of the package recursively
		_packageListDB->resolveDependencies(&install_packages, package);
	}

	{	//Go through every package and see if it is already installed
		LOGI("Checking for already installed dependencies...");
		size_t len_packages = install_packages.size();
		for (size_t i = 0; i < len_packages; i++){

			//Get the package to check
			Package* p_check = install_packages.at(i);
			LOGD("Checking if " + p_check->getName() + " is already installed...");

			//Find it in the installed db
			Package* p = _installedDB->getPackage(p_check->getName(), false);
			
			//If the package is already installed, skip its installation
			if (p != nullptr){
				LOGI(p_check->getName() + " is already installed: YES");
				install_packages.erase(install_packages.begin() + i);
				i--;
				len_packages--;
			} else {
				LOGI(p_check->getName() + " is already installed: NO");
			}
		}
	}

	LOGI("Done resolving dependencies");

	//If there is nothing to do, exit this function
	if (install_packages.size() == 0){
		LOGU("There is nothing to do");
		return;
	}

	{//Inform the user about the packages to install
		std::string msg = "Following packages will be installed:";
		for (Package* pkg : install_packages)
			msg += " " + pkg->getFullName();
		LOGU(msg);
	}

	{//Ask the user for permission
		if (!askUserOK("Do you want to continue?", true)){
			std::string msg = "Installing packages:";
			for (Package* pkg : install_packages)
				msg += " " + pkg->getFullName();
			throw new LeafError(Error::USER_DISAGREE, msg);
		}
	}

	{	//Check for the redownload config and delete cached downloads if neccesary

		LOGI("Checking for redownloads...");
		switch(lConfig.redownload){
			//If the redownload of the specified files is wanted
			case CONFIG_REDOWNLOAD_SPECIFIED:
				LOGU("Removing download cache of specified packages...");
				for (std::string package : packages){
					
					//Search for the package
					for (Package* search : install_packages){
						//If it is to be installed, remove its cache
						if (search->getName() == package){
							search->removeDownloadCache();
							break;
						}
					}
				}
				break;

			//If the redownload of all packages and dependencies is wanted
			case CONFIG_REDOWNLOAD_ALL:{
				LOGU("Removing download cache of all packages and dependencies...");
				std::error_code ec;
				std::filesystem::remove_all(lConfig.downloadDir(), ec);
				if (ec)
					throw new LeafError(Error::REMOVEDIR, "Download directory " + lConfig.downloadDir(), ec);
				break;
			}

			//Skip if there is nothing to remove
			default:
				break;
		}

		for (Package* package : install_packages){
			LOGU("Downloading package " + package->getFullName() + "...");

			if (std::filesystem::exists(package->getDownloadPath())){
				LOGI("Skipping download of package " + package->getFullName());
				continue;
			}

			package->fetch();
		}
	}

	for (Package* package : install_packages){
		LOGU("Extracting package " + package->getFullName() + "...");
		
		package->extract();
	}

	for (Package* package : install_packages){
		LOGU("Deploying package " + package->getFullName() + "...");
		
		package->deploy();
	}

	LOGU("Cleaning up package caches...");
	for (Package* package : install_packages){
		package->clearCache();
	}
}
