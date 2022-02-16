/**
 * @file		leafcore/actions.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of the Leafcore action functions (a_...)
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafcore.h"

#include "downloader.h"

#include <list>
#include <fstream>
#include <iostream>
#include <filesystem>

bool Leafcore::a_update(){
	FUN();
	_error.clear();
	if (!checkDirectories())
		return false;

	LOGI("Fetching package list from " + _pkglistURL + " to " + _pkglistFile);

	//Create the output file stream and check it
	std::ofstream file;
	file.open(_pkglistFile, std::ios::out);

	if (!file.is_open()){
		_error = "Failed to open " + _pkglistFile + " for writing";
		LOGE("Package list update: " + _error);
		file.close();
		return false;
	}

	//Create the downloader instance and download the file
	Downloader dl;
	if (!dl.init()){
		_error = "Failed to initialize downloader: " + dl.getError();
		LOGE("Package list update: " + _error);
		return false;
	}
	if (!dl.download(_pkglistURL, file)){
		_error = "Failed to download from " + _pkglistURL + ": " + dl.getError();
		LOGE("Package list update: " + _error);		
		return false;
	}
	
	file.close();

	return true;
}

bool Leafcore::a_install(std::vector<std::string> packages, bool forceDownload){
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

	std::vector<Package*> install_packages;
	LOGU("Resolving dependencies...");
	for (std::string packageName : packages){
		Package* package = _packageListDB.getPackage(packageName);

		if (package == nullptr){
			_error = "Could not find package " + packageName + " in database";
			LOGE("Failed to perform install action: " + _error);
			return false;
		}

		//Add only packages that have no empty URL
		if (!package->getFetchURL().empty())
			install_packages.push_back(package);
		else
			//If the URL is empty, but there are dependencies, treat the package as collection
			if (!package->getDependencies().empty())
				LOGU("Package " + package->getName() + " will be treated as collection");

			//Else error out
			else {
				_error = "Package " + package->getName() + " has no fetch URL and is no collection";
				LOGE("Failed to perform install action: " + _error);
				return false;
			}

		//Now resolve the dependencies of the package recursively
		if (!_packageListDB.resolveDependencies(&install_packages, package)){
			_error = "Could not resolve dependencies for package " + packageName + ": " + _packageListDB.getError();
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
		std::cout << "Do you want to continue? (Y/n): ";
		std::string answer;
		getline(std::cin, answer);
		
		if (!(answer == "y" || answer == "Y" || answer == "")){
			_error = "User aborted";
			return false;
		}
	}

	for (Package* package : install_packages){
		LOGU("Downloading package " + package->getFullName() + "...");

		bool force = false;

		//If the package is in the packages vector and it is desired, force the download
		if (forceDownload){
			for (std::string ask : packages){
				if (package->getName() == ask){
					LOGI("Forcing download of package " + package->getName());
					force = true;
					break;
				}
			}
		}

		if (!fetchPackage(package, force)){
			return false;
		}
	}

	for (Package* package : install_packages){
		LOGU("Extracting package " + package->getFullName() + "...");
		if (!extractPackage(package)){
			return false;
		}
	}

	for (Package* package : install_packages){
		LOGU("Deploying package " + package->getFullName() + "...");
		if (!deployPackage(package)){
			return false;
		}
	}

	return true;
}
