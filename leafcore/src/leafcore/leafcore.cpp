/**
 * @file		leafcore/leafcore.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of Leafcore
 * @copyright 	Copyright (c) 2022
 */

#define FRIEND_PACKAGE

#include "log.h"
#include "leafcore.h"

#include "pkglistparser.h"
#include "downloader.h"
#include "leafarchive.h"
#include "leaffs.h"

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
			msg += " " + pkg->getFullName();
		LOGU(msg);
	}

	//TODO: fetch and install packages

	for (Package* package : install_packages){
		LOGU("Downloading package " + package->getFullName() + "...");
		if (!fetchPackage(package)){
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

bool Leafcore::fetchPackage(Package* package){
	FUN();

	if (package == nullptr){
		_error = "Invalid package (nullptr)";
		LOGE("Failed to fetch package: " + _error);
		return false;
	}

	if (!std::filesystem::exists(_cachePath)){
		_error = "Cache path " + _cachePath + " does not exist";
		LOGE("Failed to fetch package " + package->getFullName() + ": " + _error);
		return false;
	}

	if (!std::filesystem::is_directory(_cachePath)){
		_error = "Cache path " + _cachePath + " is not a directory";
		LOGE("Failed to fetch package " + package->getFullName() + ": " + _error);
		return false;
	}

	if (package->getFetchURL().empty()){
		_error = "Empty package URL for package " + package->getFullName();
		LOGE("Failed to fetch package " + package->getFullName() + ": " + _error);
		return false;
	}

	std::string downloadPath = _cachePath + "downloads/";

	if (!std::filesystem::exists(downloadPath)){
		LOGI("Download path " + downloadPath + " does not exist, creating");
		if (!std::filesystem::create_directories(downloadPath)){
			_error = "Download path " + downloadPath + " could not be created";
			LOGE("Failed to fetch package " + package->getFullName() + ": " + _error);
			return false;
		}
	}
	

	std::string filePath = downloadPath + package->getFullName() + ".tar.xz";

	if (std::filesystem::exists(filePath)){
		LOGI("Skipping download of " + filePath + ", alredy existing");
		return true;
	}

	std::ofstream outFile;
	outFile.open(filePath, std::ios::binary | std::ios::out);

	if (!outFile.is_open()){
		_error = "Failed to open " + filePath + " for writing";
		LOGE("Failed to fetch package " + package->getFullName() + ": " + _error);
		return false;
	}

	Downloader dl;
	if (!dl.init()){
		_error = "Failed to initialize Downloader: " + dl.getError();
		LOGE("Failed to fetch package " + package->getFullName() + ": " + _error);
		outFile.close();
		return false;
	}

	if (!dl.download(package->getFetchURL(), outFile)){
		_error = "Failed download from " + package->getFetchURL() + ": " + dl.getError();
		LOGE("Failed to fetch package " + package->getFullName() + ": " + _error);
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

	std::string packagePath = _cachePath + "/downloads/" + package->getFullName() + ".tar.xz";

	if (!std::filesystem::exists(packagePath)){
		_error = "Package does not seem to be fetched";
		LOGE("Failed to extract package: " + _error);
		return false;
	}

	std::string extractedPath = _cachePath + "/packages/";

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

bool Leafcore::deployPackage(Package* package){
	FUN();

	if (package == nullptr){
		_error = "Invalid package (nullptr)";
		LOGE("Failed to deploy package: " + _error);
		return false;
	}

	if (!std::filesystem::exists(_rootPath)){
		_error = "Root filesystem " + _rootPath + " does not exist";
		LOGE("Failed to deploy package: " + _error);
		return false;
	}

	std::string extractedPath = _cachePath + "/packages/" + package->getFullName() + "/";

	if (!std::filesystem::exists(extractedPath)){
		_error = "Package directory " + extractedPath + " does not exist, package may not be extracted";
		LOGE("Failed to deploy package: " + _error);
		return false;
	}

	std::string dataPath = extractedPath + "/data/";

	if (!std::filesystem::exists(dataPath)){
		_error = "Data directory " + dataPath + " does not exist, package may be corrupted";
		LOGE("Failed to deploy package: " + _error);
		return false;
	}

	LeafFS fs(dataPath);

	LOGU("Indexing package " + package->getFullName() + "...");

	if (!fs.readFiles(true, true)){
		_error = "Could not index data directory: " + fs.getError();
		LOGE("Failed to deploy package: " + _error);
		return false;
	}

	package->_provided_files = fs.getFiles();

	const auto copyOptions = 	std::filesystem::copy_options::update_existing
							|	std::filesystem::copy_options::recursive;

	LOGI("Deploying package " + package->getFullName() + " to " + _rootPath);

	std::filesystem::copy(dataPath, _rootPath, copyOptions);

	LOGI("Creating .leafinstalled file...");

	{
		std::string installedDir = "/etc/leaf/installed/";

		if (!std::filesystem::exists(installedDir)){
			if (!std::filesystem::create_directories(installedDir)){
				_error = "Could not create installed directory " + extractedPath;
				LOGE("Failed to deploy package: " + _error);
				return false;
			}
		}

		std::string installedPath = installedDir + package->getName() + ".leafinstalled";

		std::ofstream file;
		file.open(installedPath, std::ios::out);

		if (!file.is_open()){
			_error = "Could not open installed file " + installedPath;
			LOGE("Failed to deploy package: " + _error);
			return false;
		}

		if (!package->createInstalledFile(file)){
			_error = "Could not create installed file " + installedPath;
			LOGE("Failed to deploy package: " + _error);
			file.close();
			return false;
		}

		file.close();
	}
	
	return true;
}
