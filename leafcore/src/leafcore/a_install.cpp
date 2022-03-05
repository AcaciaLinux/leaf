/**
 * @file		leafcore/a_install.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of Leafcore::a_install()
 * @copyright	Copyright (c) 2022
 */
#include "log.h"
#include "error.h"
#include "leafdebug.h"
#include "leafconfig.h"

#include "leafcore.h"

#include "fail.h"

#include <filesystem>

bool Leafcore::a_install(std::deque<std::string> packages, bool forceDownload){
	FUN();
	_error.clear();
	if (!checkDirectories())
		return false;

	//Check if the package list is loaded
	if (!_loadedPkgList){
		_error = "Package list is not loaded";
		LOGE("Failed to perform install action: " + _error);
		return false;
	}

	//Check if there even are some packages to process
	if (packages.size() == 0){
		_error = "At least 1 package has to be submitted";
		LOGE("Failed to perform install action: " + _error);
		return false;
	}

	{//Inform the user about the action
		std::string msg = "Installing following packages:";
		for (std::string pkg : packages)
			msg += " " + pkg;
		LOGU(msg);
	}

	std::deque<Package*> install_packages;
	LOGU("Resolving dependencies...");
	for (std::string packageName : packages){
		Package* package = _packageListDB->getPackage(packageName);

		if (package == nullptr){
			_error = "Could not find package " + packageName + " in database";
			LOGE("Failed to perform install action: " + _error);
			return false;
		}

		//Resolve the dependencies of the package recursively
		if (!_packageListDB->resolveDependencies(&install_packages, package)){
			_error = "Could not resolve dependencies for package " + packageName + ": " + _packageListDB->getError();
			LOGE(_error);
			return false;
		}
	}
	LOGI("Done resolving dependencies");

	{//Inform the user about the packages to install
		std::string msg = "Following packages will be fetched:";
		for (Package* pkg : install_packages)
			msg += " " + pkg->getFullName();
		LOGU(msg);
	}

	{//Ask the user for permission
		if (!askUserOK("Do you want to continue?", true)){
			_error = "Aborted by user";
			return FAIL(_error);
		}
	}

	for (Package* package : install_packages){
		LOGU("Downloading package " + package->getFullName() + "...");

		if (std::filesystem::exists(package->getDownloadPath())){
			LOGI("Skipping download of package " + package->getFullName());
			continue;
		}

		if (!package->fetch()){
			_error = package->getError();
			return FAIL(_error);
		}
	}

	for (Package* package : install_packages){
		LOGU("Extracting package " + package->getFullName() + "...");
		
		if (!package->extract()){
			_error = package->getError();
			return FAIL(_error);
		}
	}

	for (Package* package : install_packages){
		LOGU("Deploying package " + package->getFullName() + "...");
		
		if (!package->deploy()){
			_error = package->getError();
			return FAIL(_error);
		}
	}

	return true;
}
