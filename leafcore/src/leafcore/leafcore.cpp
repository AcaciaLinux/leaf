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

bool Leafcore::checkDirectories(){
	FUN();

	LOGD("Checking leaf directories...");

	{//Check the leaf directory
		LOGD("Checking configuration directory...");
		if (!std::filesystem::exists(lConfig.configDir())){
			if (askUserOK("Configuration directory " + lConfig.configDir() + " does not exist, create it?", true)){
				LOGU("Creating /etc/leaf directory");
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

bool Leafcore::createConfigDirs(){
	FUN();

	if (!checkDirectories())
		return false;

	{//Check the installed directory
		if (!std::filesystem::exists(lConfig.installedDir())){
			if (!std::filesystem::create_directories(lConfig.installedDir())){
				_error = "Failed to create installed directory " + lConfig.installedDir();
				return FAIL(_error);
			}
		} else {
			if (!std::filesystem::is_directory(lConfig.installedDir())){
				_error = "Installed directory " + lConfig.installedDir() + " is not a directory";
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
		LOGD("User question: Default action: true");

		std::cout << question << " (Y/n): ";
		std::string answer;
		getline(std::cin, answer);

		LOGD("User question: Answer: \"" + answer + "\"");
		
		if (answer == "y" || answer == "Y" || answer == ""){
			LOGD("User question: User agreed");
			return true;
		} else {
			LOGD("User question: User disagreed");
			return false;
		}
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
