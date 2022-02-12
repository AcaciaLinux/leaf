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
#include "leafarchive.h"

#include <fstream>
#include <filesystem>

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
		install_packages.push_back(package);
		_db.resolveDependencies(&install_packages, package);
	}
	LOGU("Done");

	{//Inform the user about the packages to install
		std::string msg = "Following packages will be fetched:";
		for (Package* pkg : install_packages)
			msg += " " + pkg->getName();
		LOGU(msg);
	}

	//TODO: fetch and install packages

	for (Package* package : install_packages){
		LOGU("Downloading package " + package->getName() + "...");
		if (!fetchPackage(package)){
			return false;
		}
	}

	for (Package* package : install_packages){
		LOGU("Extracting package " + package->getName() + "...");
		if (!extractPackage(package)){
			return false;
		}
	}

	return true;
}

bool Leafcore::fetchPackage(Package* package){
	FUN();

	if (package == nullptr){
		_error = "Invalid package (nullptr)";
		LOGE("Failed to fetch package: " + _error);
		return false;
	}

	if (!std::filesystem::exists(_cachePath)){
		_error = "Cache path " + _cachePath + " does not exist";
		LOGE("Failed to fetch package " + package->getName() + ": " + _error);
		return false;
	}

	if (!std::filesystem::is_directory(_cachePath)){
		_error = "Cache path " + _cachePath + " is not a directory";
		LOGE("Failed to fetch package " + package->getName() + ": " + _error);
		return false;
	}

	if (package->getFetchURL().empty()){
		_error = "Empty package URL for package " + package->getName();
		LOGE("Failed to fetch package " + package->getName() + ": " + _error);
		return false;
	}

	std::string downloadPath = _cachePath + "downloads/";

	if (!std::filesystem::exists(downloadPath)){
		LOGI("Download path " + downloadPath + " does not exist, creating");
		if (!std::filesystem::create_directories(downloadPath)){
			_error = "Download path " + downloadPath + " could not be created";
			LOGE("Failed to fetch package " + package->getName() + ": " + _error);
			return false;
		}
	}
	

	std::string filePath = downloadPath + package->getName() + ".tar.xz";
	std::ofstream outFile;
	outFile.open(filePath, std::ios::binary | std::ios::out);

	if (!outFile.is_open()){
		_error = "Failed to open " + filePath + " for writing";
		LOGE("Failed to fetch package " + package->getName() + ": " + _error);
		return false;
	}

	Downloader dl;
	if (!dl.init()){
		_error = "Failed to initialize Downloader: " + dl.getError();
		LOGE("Failed to fetch package " + package->getName() + ": " + _error);
		outFile.close();
		return false;
	}

	if (!dl.download(package->getFetchURL(), outFile)){
		_error = "Failed download from " + package->getFetchURL() + ": " + dl.getError();
		LOGE("Failed to fetch package " + package->getName() + ": " + _error);
		outFile.close();
		return false;
	}

	outFile.close();

	return true;
}

bool Leafcore::extractPackage(Package* package){
	FUN();

	if (package == nullptr){
		_error = "Invalid package (nullptr)";
		LOGE("Failed to extract package: " + _error);
		return false;
	}

	std::string packagePath = _cachePath + "/downloads/" + package->getName() + ".tar.xz";

	if (!std::filesystem::exists(packagePath)){
		_error = "Package does not seem to be fetched";
		LOGE("Failed to extract package: " + _error);
		return false;
	}

	std::string extractedPath = _cachePath + "/packages/" + package->getName();

	if (!std::filesystem::exists(extractedPath)){
		if (!std::filesystem::create_directories(extractedPath)){
			_error = "Could not create extracted directory " + extractedPath;
			LOGE("Failed to extract package: " + _error);
			return false;
		}
	}
	
	LeafArchive archive;
	if (!archive.load(packagePath)){
		_error = "Failed to load package into LeafArchive: " + archive.getError();
		LOGE("Failed to extract package: " + _error);
		return false;
	}
	
	if (!archive.extract(extractedPath)){
		_error = "Failed extract package using LeafArchive: " + archive.getError();
		LOGE("Failed to extract package: " + _error);
		return false;
	}

	return true;
}
