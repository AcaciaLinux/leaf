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
#include <iostream>

Leafcore::Leafcore(std::string rootDir){
	FUN();

	setRootDir(rootDir);
}

bool Leafcore::parsePackageList(){
	FUN();
	return parsePackageList(_pkglistFile);
}

bool Leafcore::parsePackageList(std::string path){
	FUN();
	_error.clear();
	if (!checkDirectories())
		return false;

	_packageListDB.clear();

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
	if (!parser.applyToDB(_packageListDB)){
		_error = "Parser apply error: " + parser.getError();
		LOGE("Failed to apply package list with " + _error);
		return false;
	}

	LOGI("Done parsing package list");
	_loadedPkgList = true;

	return true;
}

bool Leafcore::parseInstalled(){
	FUN();
	_error.clear();
	if (!checkDirectories())
		return false;

	_installedDB.clear();

	std::vector<std::string> installedFiles;

	{	//Read the directory
		LeafFS installedDir("/etc/leaf/installed/");

		if (!installedDir.check()){
			_error = "Failed to parse installed packages: " + installedDir.getError();
			LOGE(_error);
			return false;
		}

		if (!installedDir.readFiles(true, false)){
			_error = "Failed to parse installed packages: " + installedDir.getError();
			LOGE(_error);
			return false;
		}

		installedFiles = installedDir.getFiles();
	}

	if (installedFiles.size() == 0){
		LOGW("It seems that no packages are installed on the system");
		return true;
	}

	LOGD("Installed packages: ");
	for (std::string file : installedFiles){
		file.erase(0, 1);
		LOGD(" -> " + file);

		Package* newPack = _installedDB.newPackage("", "");

		std::ifstream inFile;
		inFile.open("/etc/leaf/installed/" + file + ".leafinstalled", std::ios::in);

		if (!inFile.is_open()){
			_error = "Failed to parse installed package " + file + ", failed to open file";
			LOGE(_error);
			return false;
		}

		if (!newPack->parseInstalledFile(inFile)){
			_error = "Failed to parse installed package " + file + ": " + newPack->getError();
			LOGE(_error);
			inFile.close();
			return false;
		}

		inFile.close();
	}



	return true;
}

bool Leafcore::fetchPackage(Package* package, bool forceDownload){
	FUN();
	_error.clear();
	if (!checkDirectories())
		return false;

	if (package == nullptr){
		_error = "Invalid package (nullptr)";
		LOGE("Failed to fetch package: " + _error);
		return false;
	}

	if (package->getFetchURL().empty()){
		_error = "Empty package URL for package " + package->getFullName();
		LOGE("Failed to fetch package " + package->getFullName() + ": " + _error);
		return false;
	}

	if (!std::filesystem::exists(getDownloadDir())){
		LOGI("Download path " + getDownloadDir() + " does not exist, creating");
		if (!std::filesystem::create_directories(getDownloadDir())){
			_error = "Download path " + getDownloadDir() + " could not be created";
			LOGE("Failed to fetch package " + package->getFullName() + ": " + _error);
			return false;
		}
	}

	if (std::filesystem::exists(getDownloadPath(package)) && !forceDownload){
		LOGI("Skipping fetching of package " + package->getFullName() + ", already fetched");
		return true;
	}

	std::ofstream outFile;
	outFile.open(getDownloadPath(package), std::ios::out);

	if (!outFile.is_open()){
		_error = "Failed to open " + getDownloadPath(package) + " for writing";
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
	_error.clear();
	if (!checkDirectories())
		return false;

	if (package == nullptr){
		_error = "Invalid package (nullptr)";
		LOGE("Failed to extract package: " + _error);
		return false;
	}

	if (!std::filesystem::exists(getDownloadDir())){
		_error = "Package does not seem to be fetched";
		LOGE("Failed to extract package: " + _error);
		return false;
	}

	if (!std::filesystem::exists(getExtractedDirectory(package))){
		if (!std::filesystem::create_directories(getExtractedDirectory(package))){
			_error = "Could not create extracted directory " + getExtractedDirectory(package);
			LOGE("Failed to extract package " + package->getFullName() + ": " + _error);
			return false;
		}
	}
	
	LeafArchive archive;
	if (!archive.load(getDownloadPath(package))){
		_error = "Failed to load package " + package->getFullName() + " into LeafArchive: " + archive.getError();
		LOGE("Failed to extract package: " + _error);
		return false;
	}
	
	if (!archive.extract(getPackagesDir())){
		_error = "Failed extract package " + package->getFullName() + " using LeafArchive: " + archive.getError();
		LOGE("Failed to extract package: " + _error);
		return false;
	}

	return true;
}

bool Leafcore::deployPackage(Package* package){
	FUN();
	_error.clear();
	if (!checkDirectories())
		return false;

	if (package == nullptr){
		_error = "Invalid package (nullptr)";
		LOGE("Failed to deploy package: " + _error);
		return false;
	}

	if (!std::filesystem::exists(getRootDir())){
		_error = "Root filesystem " + getRootDir() + " does not exist";
		LOGE("Failed to deploy package " + package->getFullName() + ": " + _error);
		return false;
	}

	if (!std::filesystem::exists(getExtractedDirectory(package))){
		_error = "Package directory " + getExtractedDirectory(package) + " does not exist, package may not be extracted";
		LOGE("Failed to deploy package " + package->getFullName() + ": " + _error);
		return false;
	}

	std::string dataPath = getExtractedDirectory(package) + "/data/";

	if (!std::filesystem::exists(dataPath)){
		_error = "Data directory " + dataPath + " does not exist, package may be corrupted";
		LOGE("Failed to deploy package: " + _error);
		return false;
	}

	LeafFS fs(dataPath);

	LOGI("Indexing package " + package->getFullName() + "...");

	if (!fs.readFiles(true, true)){
		_error = "Could not index data directory: " + fs.getError();
		LOGE("Failed to deploy package " + package->getFullName() + ": " + _error);
		return false;
	}

	package->_provided_files = fs.getFiles();

	const auto copyOptions = 	std::filesystem::copy_options::update_existing
							|	std::filesystem::copy_options::recursive;

	LOGI("Deploying package " + package->getFullName() + " to " + getRootDir());

	std::filesystem::copy(dataPath, getRootDir(), copyOptions);

	LOGI("Creating .leafinstalled file...");

	{
		std::string installedDir = "/etc/leaf/installed/";

		if (!std::filesystem::exists(installedDir)){
			if (!std::filesystem::create_directories(installedDir)){
				_error = "Could not create installed directory " + getExtractedDirectory(package);
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

bool Leafcore::checkDirectories(){
	FUN();

	LOGD("Checking leaf directories...");

	{//Check the leaf directory
		LOGD("Checking configuration directory...");
		if (!std::filesystem::exists(_leafDir)){
			if (askUserOK("Configuration directory " + _leafDir + " does not exist, create it?"), true){
				if (!std::filesystem::create_directories(_leafDir)){
					_error = "Could not create configuration directory " + _leafDir;
					LOGE(_error);
					return false;
				}
			}
			else
			{
				_error = "User disagreed to create configuration directory " + _leafDir;
				LOGE(_error);
				return false;
			}
		}

		if (!std::filesystem::is_directory(_leafDir)){
			_error = "Configuration directory " + _leafDir + " is not a directory";
			LOGE(_error);
			return false;
		}
	}

	{//Check the cache directory
		LOGD("Checking cache directory...");
		if (!std::filesystem::exists(_cacheDir)){
			if (askUserOK("Cache directory " + _cacheDir + " does not exist, create it?"), true){
				if (!std::filesystem::create_directories(_cacheDir)){
					_error = "Could not create cache directory " + _cacheDir;
					LOGE(_error);
					return false;
				}
			}
			else
			{
				_error = "User disagreed to create cache directory " + _cacheDir;
				LOGE(_error);
				return false;
			}
		}

		if (!std::filesystem::is_directory(_cacheDir)){
			_error = "Cache directory " + _cacheDir + " is not a directory";
			LOGE(_error);
			return false;
		}
	}

	return true;
}

bool Leafcore::askUserOK(std::string question, bool defaultOption){
	FUN();

	if (defaultOption){
		std::cout << question << " (Y/n): ";
		std::string answer;
		getline(std::cin, answer);
		
		if (answer == "y" || answer == "Y" || answer == "")
			return true;
		else 
			return false;
	} 
	else
	{
		std::cout << question << " (y/N): ";
		std::string answer;
		getline(std::cin, answer);
		
		if (answer == "n" || answer == "N" || answer == "")
			return false;
		else 
			return true;
	}
}
