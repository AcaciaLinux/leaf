/**
 * @file		leafcore/leafcore.cpp
 * @author		Max Kofler (kofler.max.dev@gmail.com)
 * @brief		The core implementation of Leafcore
 * @copyright 	Copyright (c) 2022
 */

#define FRIEND_PACKAGE

#include "log.h"
#include "fail.h"
#include "leafcore.h"
#include "leafconfig.h"

#include "pkglistparser.h"
#include "downloader.h"
#include "leafarchive.h"
#include "leaffs.h"

#include <fstream>
#include <filesystem>
#include <iostream>

leaf_config_t lConfig;

Leafcore::Leafcore(){
	FUN();

	_packageListDB = new LeafDB(this);
	_installedDB = new LeafDB(this);
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

	if (!std::filesystem::exists(lConfig.rootDir)){
		_error = "Root filesystem " + lConfig.rootDir + " does not exist";
		LOGE("Failed to deploy package " + package->getFullName() + ": " + _error);
		return false;
	}

	if (!std::filesystem::exists(package->getExtractedDir())){
		_error = "Package directory " + package->getExtractedDir() + " does not exist, package may not be extracted";
		LOGE("Failed to deploy package " + package->getFullName() + ": " + _error);
		return false;
	}

	if (!runPostInstall(package)){
		_error = "Failed preinstallation of " + package->getFullName() + ": " + _error;
		LOGE(_error);
		return false;
	}

	if (!package->copyToRoot()){
		_error = "Failed to deploy package " + package->getFullName() + ": " + package->getError();
		return FAIL(_error);
	}

	if (!runPostInstall(package)){
		_error = "Failed postinstallation of " + package->getFullName() + ": " + _error;
		LOGE(_error);
		return false;
	}

	LOGI("Creating .leafinstalled file...");

	{
		std::string installedDir = "/etc/leaf/installed/";

		if (!std::filesystem::exists(installedDir)){
			if (!std::filesystem::create_directories(installedDir)){
				_error = "Could not create installed directory " + package->getExtractedDir();
				LOGE("Failed to deploy package: " + _error);
				return false;
			}
		}

		std::string installedPath = installedDir + package->getName() + ".leafinstalled";

		std::ofstream file;
		file.open(installedPath, std::ios::trunc);

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

bool Leafcore::runPreInstall(Package* package){
	FUN();
	_error.clear();

	if (!std::filesystem::exists(package->getExtractedDir())){
		_error = "Package directory " + package->getExtractedDir() + " does not exist, package may not be extracted";
		LOGE("Failed to run preinstall of " + package->getFullName() + ": " + _error);
		return false;
	}

	if (!std::filesystem::exists(package->getExtractedDir() + "preinstall.sh")){
		LOGI("Preinstall script does not exist, skipping");
		return true;
	}

	std::string oldWorkDir = std::filesystem::current_path();

	std::filesystem::current_path(package->getExtractedDir());

	int res = 0;

	if (hlog->getLevel() < Log::I)
		res = system("bash ./preinstall.sh >> /dev/null");
	else
		res = system("bash ./preinstall.sh");

	if (res != 0){
		_error = "Preinstallation script failed to execute";
		LOGE(_error);
		return false;
	}

	std::filesystem::current_path(oldWorkDir);

	return true;
}

bool Leafcore::runPostInstall(Package* package){
	FUN();
	_error.clear();

	if (!std::filesystem::exists(package->getExtractedDir())){
		_error = "Package directory " + package->getExtractedDir() + " does not exist, package may not be extracted";
		LOGE("Failed to run postinstall of " + package->getFullName() + ": " + _error);
		return false;
	}

	if (!std::filesystem::exists(package->getExtractedDir() + "postinstall.sh")){
		LOGI("Postinstall script does not exist, skipping");
		return true;
	}

	std::string oldWorkDir = std::filesystem::current_path();

	std::filesystem::current_path(package->getExtractedDir());

	if (hlog->getLevel() < Log::I)
		system("bash ./postinstall.sh >> /dev/null");
	else
		system("bash ./postinstall.sh");

	std::filesystem::current_path(oldWorkDir);

	return true;
}

bool Leafcore::checkDirectories(){
	FUN();

	LOGD("Checking leaf directories...");

	{//Check the leaf directory
		LOGD("Checking configuration directory...");
		if (!std::filesystem::exists(lConfig.configDir())){
			if (askUserOK("Configuration directory " + lConfig.configDir() + " does not exist, create it?"), true){
				if (!std::filesystem::create_directories(lConfig.configDir())){
					_error = "Could not create configuration directory " + lConfig.configDir();
					LOGE(_error);
					return false;
				}
			}
			else
			{
				_error = "User disagreed to create configuration directory " + lConfig.configDir();
				LOGE(_error);
				return false;
			}
		}

		if (!std::filesystem::is_directory(lConfig.configDir())){
			_error = "Configuration directory " + lConfig.configDir() + " is not a directory";
			LOGE(_error);
			return false;
		}
	}

	{//Check the cache directory
		LOGD("Checking cache directory...");
		if (!std::filesystem::exists(lConfig.cacheDir())){
			if (askUserOK("Cache directory " + lConfig.cacheDir() + " does not exist, create it?"), true){
				if (!std::filesystem::create_directories(lConfig.cacheDir())){
					_error = "Could not create cache directory " + lConfig.cacheDir();
					LOGE(_error);
					return false;
				}
			}
			else
			{
				_error = "User disagreed to create cache directory " + lConfig.cacheDir();
				LOGE(_error);
				return false;
			}
		}

		if (!std::filesystem::is_directory(lConfig.cacheDir())){
			_error = "Cache directory " + lConfig.cacheDir() + " is not a directory";
			LOGE(_error);
			return false;
		}
	}

	return true;
}

bool Leafcore::createCacheDirs(){
	FUN();
	if (!checkDirectories())
		return false;

	{//Check the download directory
		if (!std::filesystem::exists(lConfig.downloadDir())){
			if (!std::filesystem::create_directories(lConfig.downloadDir())){
				_error = "Failed to create download directory " + lConfig.downloadDir();
				return FAIL(_error);
			}
		} else {
			if (!std::filesystem::is_directory(lConfig.downloadDir())){
				_error = "Download directory " + lConfig.downloadDir() + " is not a directory";
				return FAIL(_error);
			}
		}
	}
	
	{//Check the packages directory
		if (!std::filesystem::exists(lConfig.packagesDir())){
			if (!std::filesystem::create_directories(lConfig.packagesDir())){
				_error = "Failed to create packages directory " + lConfig.packagesDir();
				return FAIL(_error);
			}
		} else {
			if (!std::filesystem::is_directory(lConfig.packagesDir())){
				_error = "Packages directory " + lConfig.packagesDir() + " is not a directory";
				return FAIL(_error);
			}
		}
	}

	return true;
}

bool Leafcore::askUserOK(std::string question, bool defaultOption){
	FUN();

	if (lConfig.noAsk)
		return true;

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
