/**
 * @file		leafcore/actions.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The implementation of the Leafcore action functions (a_...)
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "fail.h"
#include "leafcore.h"
#include "leafconfig.h"

#include "downloader.h"

#include <fstream>
#include <iostream>
#include <filesystem>

bool Leafcore::a_update(){
	FUN();
	_error.clear();
	if (!checkDirectories())
		return false;

	std::string pkgListFilePath = lConfig.pkgListPath();
	LOGI("Fetching package list from " + _pkglistURL + " to " + pkgListFilePath);
	//Create the output file stream and check it
	std::ofstream file;
	file.open(pkgListFilePath, std::ios::trunc);

	if (!file.is_open()){
		_error = "Failed to open " + pkgListFilePath + " for writing";
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
		if (!deployPackage(package)){
			return false;
		}
	}

	return true;
}
