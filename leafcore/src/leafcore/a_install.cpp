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
	
	if (_config.installDependencies)
		LOGU("Resolving dependencies...");
	else
		LOGU("Not installing package dependencies!");

	for (std::string packageName : packages){
		//This throws an error if the package was not found
		Package* package = _packageListDB->getPackage(packageName, true);

		//Resolve the dependencies of the package recursively
		if (_config.installDependencies)
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

	{//Execute pre-install hooks
		LOGU("Running pre install hooks...");
		for (Hook& hook : _hooks){
			LOGD("Running pre install hook...");
			hook.execPre(_config);
		}
	}

	download_packages(pkgs_install);

	check_pkg_hashes(pkgs_install);

	install_packages(pkgs_install);

	{//Execute post-install hooks
		LOGU("Running post install hooks...");
		for (Hook& hook : _hooks){
			LOGD("Running post install hook...");
			hook.execPost(_config);
		}
	}

	if (!_config.noClean){
		LOGU("Cleaning up package caches...");
		for (Package* package : install_packages){
			package->clearCache();
		}
	}
}
