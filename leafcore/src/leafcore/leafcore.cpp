/**
 * @file		leafcore/leafcore.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of Leafcore
 * @copyright 	Copyright (c) 2022
 */

#include "log.h"
#include "leafcore.h"

#include "pkglistparser.h"
#include "downloader.h"

#include <fstream>

Leafcore::Leafcore(std::string rootPath){
	FUN();

	this->_rootPath = rootPath;
}

bool Leafcore::parsePackageList(){
	FUN();
	return parsePackageList(_pkglistFile);
}

bool Leafcore::parsePackageList(std::string path){
	FUN();
	_error = "";

	_pkglistURL = path;

	LOGI("Parsing package list " + path);

	std::ifstream file;
	file.open(path, std::ios::in);

	//Try opening package list file
	if (!file.is_open()){
		_error = "File " + path + " could not be opened";
		LOGE("Could not open package list " + path);
		file.close();
		return false;
	}

	PackageListParser parser;

	//Try parsing the file
	if (!parser.parse(file)){
		_error = "Parser error: " + parser.getError();
		LOGE("Failed to parse package list with " + _error);
		file.close();
		return false;
	}

	file.close();

	//Try to apply the file to the database
	if (!parser.applyToDB(_db)){
		_error = "Parser apply error: " + parser.getError();
		LOGE("Failed to apply package list with " + _error);
		return false;
	}

	LOGI("Done parsing package list");
	_loadedPkgList = true;

	return true;
}

bool Leafcore::a_update(){
	FUN();
	_error = "";

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

bool Leafcore::a_install(std::vector<std::string> packages){
	FUN();
	_error = "";

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
		Package* package = _db.getPackage(packageName);

		if (package == nullptr){
			_error = "Could not find package " + packageName + " in database";
			LOGE("Failed to perform install action: " + _error);
			return false;
		}

		_db.resolveDependencies(&install_packages, package);
	}
	LOGU("Done");

	{//Inform the user about the packages to install
		std::string msg = "Following dependencies are needed:";
		for (Package* pkg : install_packages)
			msg += " " + pkg->getName();
		LOGU(msg);
	}

	//TODO: fetch and install packages

	return true;
}
